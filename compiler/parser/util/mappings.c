#include "../../../core/log.h"
#include "../../../core/colors.h"
#include "../../unit.h"
#include "../util/hash.h"
#include "../util/arrays.h"
#include <setjmp.h>
#include <stdio.h>
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

static void push(hash_map* i, int hash, int val)
{
    if (i->len != i->values.len || i->len != i->keys.len)
        panic("Lookup table lengths dont match (H:V:K) (%3llu:%llu:%-3llu)\n", i->len, i->values.len, i->keys.len);

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

    add(maps.instr, "MOV", MOV)  ; add(maps.instr, "MVI", MVI)  ; add(maps.instr, "LXI", LXI)  ; add(maps.instr, "LDA", LDA) ; add(maps.instr, "STA", STA)  ;
    add(maps.instr, "LHLD", LHLD); add(maps.instr, "HLD", SHLD) ; add(maps.instr, "DAX", LDAX) ; add(maps.instr, "TAX", STAX); add(maps.instr, "CHG", XCHG) ;
    add(maps.instr, "ADD", ADD)  ; add(maps.instr, "ADI", ADI)  ; add(maps.instr, "ADC", ADC)  ; add(maps.instr, "ACI", ACI) ; add(maps.instr, "SUB", SUB)  ;
    add(maps.instr, "SUI", SUI)  ; add(maps.instr, "SBB", SBB)  ; add(maps.instr, "SBI", SBI)  ; add(maps.instr, "INR", INR) ; add(maps.instr, "DCR", DCR)  ;
    add(maps.instr, "INX", INX)  ; add(maps.instr, "DCX", DCX)  ; add(maps.instr, "DAD", DAD)  ; add(maps.instr, "DAA", DAA) ; add(maps.instr, "ANA", ANA)  ;
    add(maps.instr, "ANI", ANI)  ; add(maps.instr, "ORA", ORA)  ; add(maps.instr, "ORI", ORI)  ; add(maps.instr, "XRA", XRA) ; add(maps.instr, "XRI", XRI)  ;
    add(maps.instr, "CMP", CMP)  ; add(maps.instr, "CPI", CPI)  ; add(maps.instr, "RLC", RLC)  ; add(maps.instr, "RRC", RRC) ; add(maps.instr, "RAL", RAL)  ;
    add(maps.instr, "RAR", RAR)  ; add(maps.instr, "CMA", CMA)  ; add(maps.instr, "CMC", CMC)  ; add(maps.instr, "STC", STC) ; add(maps.instr, "JMP", JMP)  ;
    add(maps.instr, "Jccc", Jccc); add(maps.instr, "CALL", CALL); add(maps.instr, "Cccc", Cccc); add(maps.instr, "RET", RET) ; add(maps.instr, "Rccc", Rccc);
    add(maps.instr, "RST", RST)  ; add(maps.instr, "PCHL", PCHL); add(maps.instr, "PUSH", PUSH); add(maps.instr, "POP", POP) ; add(maps.instr, "XTHL", XTHL);
    add(maps.instr, "SPHL", SPHL); add(maps.instr, "IN", IN)    ; add(maps.instr, "OUT", OUT)  ; add(maps.instr, "EI", EI)   ; add(maps.instr, "DI", DI)    ;
    add(maps.instr, "HLT", HLT)  ; add(maps.instr, "NOP", NOP)  ;

    add(maps.reg, "A", REG_A   ); add(maps.reg, "B", REG_B  );
    add(maps.reg, "C", REG_C   ); add(maps.reg, "D", REG_D  );
    add(maps.reg, "E", REG_E   ); add(maps.reg, "H", REG_H  );
    add(maps.reg, "L", REG_L   ); add(maps.reg, "M", REG_M  );

    add(maps.rp, "BC",  RP_BC  ); add(maps.rp, "DE", RP_DE );
    add(maps.rp, "HL",  RP_HL  ); add(maps.rp, "SP", RP_SP );
    add(maps.rp, "PSW", RP_PSW);

    add(maps.cnd, "NZ", RC_NZ ); add(maps.cnd, "Z",   RC_Z);
    add(maps.cnd, "NC", RC_NC ); add(maps.cnd, "_C", RC_C);
    add(maps.cnd, "PO", RC_PO ); add(maps.cnd, "PE", RC_PE);
    add(maps.cnd, "P",  RC_P); add(maps.cnd, "_M", RC_M);
}

int find(const char* what, const char* original, int hash, hash_map* sect)
{
    for ( size_t i = 0; i < sect->len; ++i )
    {
        WHEN_VERBOSE(
            debug(
                "Map %11s %3s (Index %2lu) %13d VS %-13d (%d)",
                what,
                sect->keys.items[i] == hash? "HIT" : "MISS",
                i,
                sect->keys.items[i],
                hash,
                sect->values.items[i]));

        if (sect->keys.items[i] == hash)
            return sect->values.items[i];
    }

    error("'%s' is not a valid %s.\n", original, what);
    return -1;
}

int hash_search(const char* what, const char* name, hash_map* table)
{
    uint32_t hsh = hash(name);

    if (hsh % 2 == 1)
        return find(what, name, hsh, table + 1);
    else
        return find(what, name, hsh, table + 0);
}

int str_to_rp(const char* name)
{
    if (maps.rp[0].len == 0)
    {
        WHEN_VERBOSE (printf(YELLOW"WARN"RESET": Lookup tables weren't initialized before search.\n"));
        init_maps();
    }

    return hash_search("register pair", name[0] == '$'? name + 1 : name, maps.rp);
}

int str_to_reg(const char* name)
{
    if (maps.reg[0].len == 0)
    {
        WHEN_VERBOSE (printf(YELLOW"WARN"RESET": Lookup tables weren't initialized before search.\n"));
        init_maps();
    }

    return hash_search("register", name[0] == '$'? name + 1 : name, maps.reg);
}

int str_to_instr(const char* name)
{
    if (maps.instr[0].len == 0)
    {
        WHEN_VERBOSE (warn("Lookup tables weren't initialized before search.\n"));
        init_maps();
    }

    return hash_search("instruction", name, maps.instr);
}