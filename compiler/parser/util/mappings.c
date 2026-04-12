#include "../../../core/log.h"
#include "../../unit.h"
#include "../util/hash.h"
#include "../util/arrays.h"
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t len;

    int_arr keys;
    int_arr values;
} hash_map;

static struct {
    hash_map instr[2];
    hash_map reg[2];
    hash_map rp[2];
    hash_map cnd[2];

    jmp_buf jmp;
} maps;

static void push(hash_map* i, int hash, enum instruction_kind val)
{
    if (i->len != i->values.len || i->len != i->keys.len)
        panic("Instruction table lengths dont match (H:V:K) (%3llu:%llu:%-3llu)", i->len, i->values.len, i->keys.len);

    push_int(&i->values, val);
    push_int(&i->keys, hash);

    i->len++;
}

static void add(hash_map in[2], char* asstr, int val)
{
    uint32_t h = hash(asstr);

    if (h % 2)
        push(&in[1], h, val);
    else
        push(&in[0], h, val);
}

void init_maps()
{
    memset(&maps.instr[0], 0, sizeof (hash_map));
    memset(&maps.instr[1], 0, sizeof (hash_map));

    add(maps.instr, "MOV", MOV)   ; add(maps.instr, "MVI", MVI)   ; add(maps.instr, "LXI", LXI)   ; add(maps.instr, "LDA", LDA)  ; add(maps.instr, "STA", STA)   ;
    add(maps.instr, "LHLD", LHLD) ; add(maps.instr, "HLD", SHLD)  ; add(maps.instr, "DAX", LDAX)  ; add(maps.instr, "TAX", STAX) ; add(maps.instr, "CHG", XCHG)  ;
    add(maps.instr, "ADD", ADD)   ; add(maps.instr, "ADI", ADI)   ; add(maps.instr, "ADC", ADC)   ; add(maps.instr, "ACI", ACI)  ; add(maps.instr, "SUB", SUB)   ;
    add(maps.instr, "SUI", SUI)   ; add(maps.instr, "SBB", SBB)   ; add(maps.instr, "SBI", SBI)   ; add(maps.instr, "INR", INR)  ; add(maps.instr, "DCR", DCR)   ;
    add(maps.instr, "INX", INX)   ; add(maps.instr, "DCX", DCX)   ; add(maps.instr, "DAD", DAD)   ; add(maps.instr, "DAA", DAA)  ; add(maps.instr, "ANA", ANA)   ;
    add(maps.instr, "ANI", ANI)   ; add(maps.instr, "ORA", ORA)   ; add(maps.instr, "ORI", ORI)   ; add(maps.instr, "XRA", XRA)  ; add(maps.instr, "XRI", XRI)   ;
    add(maps.instr, "CMP", CMP)   ; add(maps.instr, "CPI", CPI)   ; add(maps.instr, "RLC", RLC)   ; add(maps.instr, "RRC", RRC)  ; add(maps.instr, "RAL", RAL)   ;
    add(maps.instr, "RAR", RAR)   ; add(maps.instr, "CMA", CMA)   ; add(maps.instr, "CMC", CMC)   ; add(maps.instr, "STC", STC)  ; add(maps.instr, "JMP", JMP)   ;
    add(maps.instr, "Jccc", Jccc) ; add(maps.instr, "CALL", CALL) ; add(maps.instr, "Cccc", Cccc) ; add(maps.instr, "RET", RET)  ; add(maps.instr, "Rccc", Rccc) ;
    add(maps.instr, "RST", RST)   ; add(maps.instr, "PCHL", PCHL) ; add(maps.instr, "PUSH", PUSH) ; add(maps.instr, "POP", POP)  ; add(maps.instr, "XTHL", XTHL) ;
    add(maps.instr, "SPHL", SPHL) ; add(maps.instr, "IN", IN)     ; add(maps.instr, "OUT", OUT)   ; add(maps.instr, "EI", EI)    ; add(maps.instr, "DI", DI)     ;
    add(maps.instr, "HLT", HLT)   ; add(maps.instr, "NOP", NOP)   ;

    add(maps.reg, "A", A)   ; add(maps.reg, "B", B)  ;
    add(maps.reg, "C", C)   ; add(maps.reg, "D", D)  ;
    add(maps.reg, "E", E)   ; add(maps.reg, "H", H)  ;
    add(maps.reg, "L", L)   ; add(maps.reg, "M", M)  ;

    add(maps.rp, "BC", BC)  ; add(maps.rp, "DE", DE) ;
    add(maps.rp, "HL", HL)  ; add(maps.rp, "SP", SP) ;
    add(maps.rp, "PSW", PSW);

    add(maps.cnd, "NZ", NZ) ; add(maps.cnd, "Z", Z)  ;
    add(maps.cnd, "NC", NC) ; add(maps.cnd, "_C", _C);
    add(maps.cnd, "PO", PO) ; add(maps.cnd, "PE", PE);
    add(maps.cnd, "P", P)   ; add(maps.cnd, "_M", _M);
    }

int bfind(int hash, hash_map* sect)
{
    for ( int i = 0, j = sect->len; i <= j; )
    {
        debug("I %d VS %d", sect->keys.items[i], hash);
        debug("J %d VS %d", sect->keys.items[j], hash);
        if (sect->keys.items[i++] == hash)
            return sect->values.items[i - 1];
        else if (sect->keys.items[j--] == hash)
            return sect->values.items[j + 1];
    }

    longjmp(maps.jmp, 1);
}

int hash_search(char* name, hash_map* table)
{
    int hsh = hash(name[0] == '$'? name + 1 : name); // purge register identifier $

    if (hsh % 2)
        return bfind(hsh, table + 1);
    else
        return bfind(hsh, table + 0);
}

enum reg_pair str_to_rp(char* name)
{
    if (maps.rp[0].len == 0)
    {
        warn("Lookup tables weren't initialized before search.");
        init_maps();
    }

    if (setjmp(maps.jmp))
        error("'%s' is not a valid register pair", name);

    return hash_search(name, maps.rp);
}

enum reg str_to_reg(char* name)
{
    if (maps.reg[0].len == 0)
    {
        warn("Lookup tables weren't initialized before search.");
        init_maps();
    }

    if (setjmp(maps.jmp))
        error("'%s' is not a valid register", name);

    return hash_search(name, maps.reg);
}

enum instruction_kind str_to_instr(char* name)
{
    if (maps.instr[0].len == 0)
    {
        warn("Instruction lookup tables weren't initialized before search.");
        init_maps();
    }

    if (setjmp(maps.jmp))
        error("'%s' is not a valid instruction", name);

    return hash_search(name, maps.instr);
}