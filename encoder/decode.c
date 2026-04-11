#include "../../core/log.h"
#include "decode.h"
#include "masks.h"
#include "resolve.h"
#include <stdio.h>
#include <stdlib.h>

#define BITMASK(orig, mask) \
    (((orig) & (mask)) == (mask))

static instruction* out = NULL;
static byte opcode = 0;
static char atlas[][5] = {
    "MOV" , "MVI" , "LXI" , "LDA" , "STA" ,
    "LHLD", "SHLD", "LDAX", "STAX", "XCHG",
    "ADD" , "ADI" , "ADC" , "ACI" , "SUB" ,
    "SUI" , "SBB" , "SBI" , "INR" , "DCR" ,
    "INX" , "DCX" , "DAD" , "DAA" , "ANA" ,
    "ANI" , "ORA" , "ORI" , "XRA" , "XRI" ,
    "CMP" , "CPI" , "RLC" , "RRC" , "RAL" ,
    "RAR" , "CMA" , "CMC" , "STC" , "JMP" ,
    "Jccc", "CALL", "Cccc", "RET" , "Rccc",
    "RST" , "PCHL", "PUSH", "POP" , "XTHL",
    "SPHL", "IN"  , "OUT" , "EI"  , "DI"  ,
    "HLT" , "NOP"
};

void dump_decoder()
{
    printf("+ Dump decoder pipes:\n");
    if (out == NULL)
        printf("!! Null (No instruction decoded)\n");
    else {
        printf("| > %s <\n", atlas[out->kind]);
        printf("| A: %-2X B: %-2X\n", *out->opA, *out->opB);
        printf("| Opcode:   %d\n", opcode);
    }
}

const char* idx_tostr(size_t idx)
{
    return atlas[idx];
}

instruction *decode(cpu* c, int mempos, size_t tick)
{
    if (out == NULL)
    {
        if (tick != 0)
            error("Someone deleted the decode buffer (null despite tick %lu)", tick);

        out = calloc(1, sizeof (instruction));
    }

    word* truepos = mempos == -1? &c->rgf.PC : (word*)(&mempos);
    opcode = c->memory[(*truepos)++];

    if (out->opcode == opcode)
        return out;

    out->opcode = opcode;

    if (BITMASK(opcode, m_MOV)) {
        out->kind = MOV;
        out->opA = resolve(c, (byte)((opcode & DF_REG_MASK) << 2));
        out->opB = resolve(c, (byte)((opcode & DF_REG_MASK) << 5));
    }
    else if (BITMASK(opcode, m_MVI)) {
        out->kind = MVI;
        out->opA = resolve(c, (byte)((opcode & DF_REG_MASK) << 2));
        out->opB = &c->memory[(*truepos)++];
    }
    else if (BITMASK(opcode, m_LXI)) {
        out->kind = LXI;
        out->opA = resolve(c, (byte)((opcode & DF_REG_MASK) << 2));
        out->opB = &c->memory[(*truepos)++]; (*truepos)++;
    }
    else if (BITMASK(opcode, m_LDA)) {
        out->kind = LDA;
        out->opA = &c->memory[(*truepos)++]; (*truepos)++;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_STA)) {
        out->kind = STA;
        out->opA = &c->memory[(*truepos)++]; (*truepos)++;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_LHLD)) {
        out->kind = LHLD;
        out->opA = &c->memory[(*truepos)++]; (*truepos)++;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_SHLD)) {
        out->kind = SHLD;
        out->opA = &c->memory[(*truepos)++]; (*truepos)++;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_LDAX)) {
        out->kind = LDAX;
        out->opA = resolve_rp_xx(c, (byte)((opcode & DF_RP_MASK) << 2));
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_STAX)) {
        out->kind = STAX;
        out->opA = resolve_rp_xx(c, (byte)((opcode & DF_RP_MASK) << 2));
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_XCHG)) {
        out->kind = XCHG;
        out->opA = NULL;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_ADD)) {
        out->kind = ADD;
        out->opA = resolve(c, (byte)((opcode & DF_REG_MASK) << 5));
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_ADI)) {
        out->kind = ADI;
        out->opA = &c->memory[(*truepos)++];
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_ADC)) {
        out->kind = ADC;
        out->opA = resolve(c, (byte)((opcode & DF_REG_MASK) << 5));
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_ACI)) {
        out->kind = ACI;
        out->opA = &c->memory[(*truepos)++];
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_SUB)) {
        out->kind = SUB;
        out->opA = resolve(c, (byte)((opcode & DF_REG_MASK) << 5));
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_SUI)) {
        out->kind = SUI;
        out->opA = &c->memory[(*truepos)++];
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_SBB)) {
        out->kind = SBB;
        out->opA = resolve(c, (byte)((opcode & DF_REG_MASK) << 5));
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_SBI)) {
        out->kind = SBI;
        out->opA = &c->memory[(*truepos)++];
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_INR)) {
        out->kind = INR;
        out->opA = resolve(c, (byte)((opcode & DF_REG_MASK) << 2));
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_DCR)) {
        out->kind = DCR;
        out->opA = resolve(c, (byte)((opcode & DF_REG_MASK) << 2));
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_INX)) {
        out->kind = INX;
        out->opA = resolve_rp(c, (byte)((opcode & DF_RP_MASK) << 2));
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_DCX)) {
        out->kind = DCX;
        out->opA = resolve_rp(c, (byte)((opcode & DF_RP_MASK) << 2));
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_DAD)) {
        out->kind = DAD;
        out->opA = resolve_rp(c, (byte)((opcode & DF_RP_MASK) << 2));
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_DAA)) {
        out->kind = DAA;
        out->opA = NULL;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_ANA)) {
        out->kind = ANA;
        out->opA = resolve(c, (byte)((opcode & DF_RP_MASK) << 5));
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_ANI)) {
        out->kind = ANI;
        out->opA = &c->memory[(*truepos)++];
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_ORA)) {
        out->kind = ORA;
        out->opA = resolve(c, (byte)((opcode & DF_RP_MASK) << 5));
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_ORI)) {
        out->kind = ORI;
        out->opA = &c->memory[(*truepos)++];
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_XRA)) {
        out->kind = XRA;
        out->opA = resolve(c, (byte)((opcode & DF_RP_MASK) << 5));
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_XRI)) {
        out->kind = XRI;
        out->opA = &c->memory[(*truepos)++];
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_CMP)) {
        out->kind = CMP;
        out->opA = resolve(c, (byte)((opcode & DF_REG_MASK) << 5));
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_CPI)) {
        out->kind = CPI;
        out->opA = &c->memory[(*truepos)++];
        out->opB = NULL;
    }
    else if (BITMASK(opcode, DF_REG_MASK)) {
        out->kind = RLC;
        out->opA = NULL;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_RRC)) {
        out->kind = RRC;
        out->opA = NULL;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_RAL)) {
        out->kind = RAL;
        out->opA = NULL;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_RAR)) {
        out->kind = RAR;
        out->opA = NULL;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_CMA)) {
        out->kind = CMA;
        out->opA = NULL;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_CMC)) {
        out->kind = CMC;
        out->opA = NULL;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_STC)) {
        out->kind = STC;
        out->opA = NULL;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_JMP)) {
        out->kind = JMP;
        out->opA = &c->memory[(*truepos)++];
        out->opB = &c->memory[(*truepos)++];
    }
    else if (BITMASK(opcode, m_Jccc)) {
        out->kind = Jccc;
        out->opA = (byte*)resolve_condition((byte)((opcode & DF_REG_MASK) << 2));
        out->opB = &c->memory[(*truepos)++]; (*truepos)++;
    }
    else if (BITMASK(opcode, m_CALL)) {
        out->kind = CALL;
        out->opA = &c->memory[(*truepos)++];
        out->opB = &c->memory[(*truepos)++];
    }
    else if (BITMASK(opcode, m_Cccc)) {
        out->kind = Cccc;
        out->opA = (byte*)resolve_condition((byte)((opcode & DF_REG_MASK) << 2));
        out->opB = &c->memory[(*truepos)++]; (*truepos)++;
    }
    else if (BITMASK(opcode, m_RET)) {
        out->kind = RET;
        out->opA = NULL;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_Rccc)) {
        out->kind = Rccc;
        out->opA = (byte*)resolve_condition((byte)((opcode & DF_REG_MASK) << 2));
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_RST)) {
        panic_ext(c, "RST: Not sure what this is supposed to be");
    }
    else if (BITMASK(opcode, m_PCHL)) {
        out->kind = PCHL;
        out->opA = NULL;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_PUSH)) {
        out->kind = PUSH;
        out->opA = resolve_stack_rp(c, (byte)((opcode & DF_RP_MASK) << 2));
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_POP)) {
        out->kind = POP;
        out->opA = resolve_stack_rp(c, (byte)((opcode & DF_RP_MASK) << 2));
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_XTHL)) {
        out->kind = XTHL;
        out->opA = NULL;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_SPHL)) {
        out->kind = SPHL;
        out->opA = NULL;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_IN)) {
        out->kind = IN;
        out->opA = NULL;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_OUT)) {
        out->kind = OUT;
        out->opA = NULL;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_EI)) {
        out->kind = EI;
        out->opA = NULL;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_DI)) {
        out->kind = DI;
        out->opA = NULL;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_HLT)) {
        out->kind = HLT;
        out->opA = NULL;
        out->opB = NULL;
    }
    else if (BITMASK(opcode, m_NOP)) {
        out->kind = NOP;
        out->opA = NULL;
        out->opB = NULL;
    }
    else panic_ext(c, "Undefined instruction %#x", opcode);

    return out;
}