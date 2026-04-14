#include <stdio.h>
#include <stdlib.h>
#include "util/masks.h"
#include "../unit.h"
#include "argument/argument.h"
#include "../../core/log.h"
#include "../../core/common.h"

#define BITMASK(orig, mask) \
    (((orig) & (mask)) == (mask))


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

char* itos(int in)
{
    static char buff[15];
    snprintf(buff, 15, "%d", in);

    return buff;
}

#define _A (1)
#define _B (1 << 1)
#define _C (1 << 2)

void expects(comp_unit* i, char* fields, int flags)
{
    if ((flags & _A && i->opA == NULL) ||
        (flags & _B && i->opB == NULL) ||
        (flags & _C && i->opC == NULL))
        goto fail;

    return;

fail:
    error ("+ Invalid arguments for instruction '%s'\n",
        atlas[i->type]);
    printf("| Expected (%s)\n", fields);
    printf("| Got      (%s, %s, %s)\n",
        i->opA? itos( * i->opA ) : "NULL",
        i->opB? itos( * i->opB ) : "NULL",
        i->opC? itos( * i->opC ) : "NULL");
    exit(1);
}

byte encode(comp_unit* in)
{
    byte out;

    switch (in->type)
    {
        case MOV:
        expects(in, "RDest, RSrc", _A | _B);
            out = m_MOV;
            out |= encode_register(*in->opA) << 4;
            out |= encode_register(*in->opB);
            break;
        case MVI:
        expects(in, "Register, byte", _A | _B);
            out = m_MVI;
            out |= encode_register(*in->opA) << 4;
            *in->opA = *in->opB;
            break;
        case LXI:
        expects(in, "Register, MemLo, MemHi", _A | _B | _C);
            out = m_LXI;
            out |= encode_regpair(*in->opA) << 4;
            *in->opA = *in->opB;
            *in->opB = *in->opC;
            break;
        case LDA:
        expects(in, "MemLo, MemHi", _A | _B);
            out = m_LDA;
            break;
        case STA:
        expects(in, "MemLo, MemHi", _A | _B);
            out = m_STA;
            break;
        case LHLD:
        expects(in, "MemLo, MemHi", _A | _B);
            out = m_LHLD;
            break;
        case SHLD:
        expects(in, "MemLo, MemHi", _A | _B);
            out = m_SHLD;
            break;
        case LDAX:
        expects(in, "RegPair, MemLo, MemHi", _A | _B | _C);
            out = m_LDAX;
            out |= encode_rp_xx(*in->opA) << 4;
            *in->opA = *in->opB;
            *in->opB = *in->opC;
            break;
        case STAX:
        expects(in, "RegPair, MemLo, MemHi", _A | _B | _C);
            out = m_STAX;
            out |= encode_rp_xx(*in->opA) << 4;
            *in->opA = *in->opB;
            *in->opB = *in->opC;
            break;
        case XCHG:
        expects(in, "Nothing", 0);
            out = m_XCHG;
            break;
        case ADD:
        expects(in, "Register", REG_A);
            out = m_ADD;
            out |= encode_register(*in->opA);
            break;
        case ADI:
        expects(in, "Byte", REG_A);
            out = m_ADI;
            break;
        case ADC:
        expects(in, "Register", REG_A);
            out = m_ADC;
            out |= encode_register(*in->opA);
            break;
        case ACI:
        expects(in, "Byte", REG_A);
            out = m_ACI;
            break;
        case SUB:
        expects(in, "Register", REG_A);
            out = m_SUB;
            out |= encode_register(*in->opA);
            break;
        case SUI:
        expects(in, "Byte", REG_A);
            out = m_SUI;
            break;
        case SBB:
        expects(in, "Register", REG_A);
            out = m_SBB;
            out |= encode_register(*in->opA);
            break;
        case SBI:
        expects(in, "Byte", REG_A);
            out = m_SBI;
            break;
        case INR:
        expects(in, "Register", REG_A);
            out = m_INR;
            out |= encode_register(*in->opA) << 4;
            break;
        case DCR:
        expects(in, "Register", REG_A);
            out = m_DCR;
            out |= encode_register(*in->opA) << 4;
            break;
        case INX:
        expects(in, "Register", REG_A);
            out = m_INR;
            out |= encode_regpair(*in->opA) << 4;
            break;
        case DCX:
        expects(in, "Register", REG_A);
            out = m_INR;
            out |= encode_regpair(*in->opA) << 4;
            break;
        case DAD:
        expects(in, "RegisterPair", REG_A);
            out = m_DAD;
            out |= encode_regpair(*in->opA) << 4;
            break;
        case DAA:
        expects(in, "Nothing", 0);
            out = m_DAA;
            break;
        case ANA:
        expects(in, "Register", REG_A);
            out = m_ANA;
            out |= encode_register(*in->opA) << 4;
            break;
        case ANI:
        expects(in, "Byte", REG_A);
            out = m_ANI;
            break;
        case ORA:
        expects(in, "Register", REG_A);
            out = m_ORA;
            out |= encode_register(*in->opA) << 4;
            break;
        case ORI:
        expects(in, "Byte", REG_A);
            out = m_ORI;
            break;
        case XRA:
        expects(in, "Register", REG_A);
            out = m_XRA;
            out |= encode_register(*in->opA) << 4;
            break;
        case XRI:
        expects(in, "Byte", REG_A);
            out = m_XRI;
            break;
        case CMP:
        expects(in, "Register", REG_A);
            out = m_CMP;
            out |= encode_register(*in->opA);
            break;
        case CPI:
        expects(in, "Byte", REG_A);
            out = m_ORI;
            break;
        case RLC:
        expects(in, "Nothing", 0);
            out = m_RLC;
            break;
        case RRC:
        expects(in, "Nothing", 0);
            out = m_RRC;
            break;
        case RAL:
        expects(in, "Nothing", 0);
            out = m_RAL;
            break;
        case RAR:
        expects(in, "Nothing", 0);
            out = m_RLC;
            break;
        case CMA:
        expects(in, "Nothing", 0);
            out = m_CMA;
            break;
        case CMC:
        expects(in, "Nothing", 0);
            out = m_CMC;
            break;
        case STC:
        expects(in, "Nothing", 0);
            out = m_STC;
            break;
        case JMP:
        expects(in, "MemLo, MemHi", _A | _B | _C);
            out = m_JMP;
            break;
        case Jccc:
        expects(in, "MemLo, MemHi", _A | _B | _C);
            out = m_Jccc;
            break;
        case CALL:
        expects(in, "MemLo, MemHi", _A | _B | _C);
            out = m_CALL;
            break;
        case Cccc:
        expects(in, "MemLo, MemHi", _A | _B | _C);
            out = m_Cccc;
            break;
        case RET:
        expects(in, "Nothing", 0);
            out = m_RET;
            break;
        case Rccc:
        expects(in, "Condition", REG_A);
            out = m_Rccc;
            out |= encode_cond(*in->opA);
            break;
        case RST:
        expects(in, "Nothing", 0);
            warn ("RST: i8080-emu does not implement this instruction and will panic");
            out = m_RST;
            break;
        case PCHL:
        expects(in, "Nothing", 0);
            out = m_PCHL;
            break;
        case PUSH:
        expects(in, "RegPair", REG_A);
            out = m_PUSH;
            out |= encode_regpair(*in->opA) << 4;
            break;
        case POP:
            out = m_POP;
            out |= encode_regpair(*in->opA) << 4;
            break;
        case XTHL:
        expects(in, "Nothing", 0);
            out = m_XTHL;
            break;
        case SPHL:
        expects(in, "Nothing", 0);
            out = m_SPHL;
            break;
        case IN:
        expects(in, "Port", REG_A);
            warn("IN: i8080-emu does not implement this instruction and will panic");
            out = m_IN;
            break;
        case OUT:
        expects(in, "Port", REG_A);
            warn("IN: i8080-emu does not implement this instruction and will panic");
            out = m_OUT;
            break;
        case EI:
        expects(in, "Nothing", 0);
            out = m_EI;
            break;
        case DI:
        expects(in, "Nothing", 0);
            out = m_DI;
            break;
        case HLT:
        expects(in, "Nothing", 0);
            out = m_HLT;
            break;
        case NOP:
        expects(in, "Nothing", 0);
            out = m_NOP;
            break;
        default:
            error("Unknown instruction %d", in->type);
            exit(1);
    }

    return out;
}