#include <stdio.h>
#include <stdlib.h>
#include "masks.h"
#include "../unit.h"
#include "argument.h"
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

#define EXPECTS(cu, fields, A, B, C) \
    do {                                                        \
        if (!(A && B && C))                                     \
        {                                                       \
            error ("+ Invalid arguments for instruction '%s'\n",\
                atlas[cu.type]);                                \
            printf("| Expected (" fields ")\n");                \
            printf("| Got      (%s, %s, %s)\n",                 \
                A? itos( A->value ) : "NULL",                   \
                B? itos( B->value ) : " ",                      \
                C? itos( C->value ) : " ");                     \
            exit(1);                                            \
        }                                                       \
    } while(0)                                                  \

byte encode(comp_unit in)
{
    byte out;

    switch (in.type)
    {
        case MOV:
        EXPECTS(in, "RDest, RSrc", in.opA, in.opB, in.opC);
            out = m_MOV;
            out |= encode_register(in.opA->value) >> 2;
            out |= encode_register(in.opB->value) >> 5;
            break;
        case MVI:
        EXPECTS(in, "Register, byte", in.opA, in.opB, in.opC);
            out = m_MVI;
            out |= encode_register(in.opA->value) >> 2;
            *in.opA = *in.opB;
            break;
        case LXI:
        EXPECTS(in, "Register, MemLo, MemHi", in.opA, in.opB, in.opC);
            out = m_LXI;
            out |= encode_regpair(in.opA->value) >> 2;
            *in.opA = *in.opB;
            *in.opB = *in.opC;
            break;
        case LDA:
        EXPECTS(in, "MemLo, MemHi", in.opA, in.opB, in.opC);
            out = m_LDA;
            break;
        case STA:
        EXPECTS(in, "MemLo, MemHi", in.opA, in.opB, in.opC);
            out = m_STA;
            break;
        case LHLD:
        EXPECTS(in, "MemLo, MemHi", in.opA, in.opB, in.opC);
            out = m_LHLD;
            break;
        case SHLD:
        EXPECTS(in, "MemLo, MemHi", in.opA, in.opB, in.opC);
            out = m_SHLD;
            break;
        case LDAX:
        EXPECTS(in, "RegPair, MemLo, MemHi", in.opA, in.opB, in.opC);
            out = m_LDAX;
            out |= encode_rp_xx(in.opA->value) >> 2;
            *in.opA = *in.opB;
            *in.opB = *in.opC;
            break;
        case STAX:
        EXPECTS(in, "RegPair, MemLo, MemHi", in.opA, in.opB, in.opC);
            out = m_STAX;
            out |= encode_rp_xx(in.opA->value) >> 2;
            *in.opA = *in.opB;
            *in.opB = *in.opC;
            break;
        case XCHG:
        EXPECTS(in, "Nothing", !in.opA, !in.opB, !in.opC);
            out = m_XCHG;
            break;
        case ADD:
        EXPECTS(in, "Register", in.opA, !in.opB, !in.opC);
            out = m_ADD;
            out |= encode_register(in.opA->value) >> 5;
            break;
        case ADI:
        EXPECTS(in, "Byte", in.opA, !in.opB, !in.opC);
            out = m_ADI;
            break;
        case ADC:
        EXPECTS(in, "Register", in.opA, !in.opB, !in.opC);
            out = m_ADC;
            out |= encode_register(in.opA->value) >> 5;
            break;
        case ACI:
        EXPECTS(in, "Byte", in.opA, !in.opB, !in.opC);
            out = m_ACI;
            break;
        case SUB:
        EXPECTS(in, "Register", in.opA, !in.opB, !in.opC);
            out = m_SUB;
            out |= encode_register(in.opA->value) >> 5;
            break;
        case SUI:
        EXPECTS(in, "Byte", in.opA, !in.opB, !in.opC);
            out = m_SUI;
            break;
        case SBB:
        EXPECTS(in, "Register", in.opA, !in.opB, !in.opC);
            out = m_SBB;
            out |= encode_register(in.opA->value) >> 5;
            break;
        case SBI:
        EXPECTS(in, "Byte", in.opA, !in.opB, !in.opC);
            out = m_SBI;
            break;
        case INR:
        EXPECTS(in, "Register", in.opA, !in.opB, !in.opC);
            out = m_INR;
            out |= encode_register(in.opA->value) >> 2;
            break;
        case DCR:
        EXPECTS(in, "Register", in.opA, !in.opB, !in.opC);
            out = m_DCR;
            out |= encode_register(in.opA->value) >> 2;
            break;
        case INX:
        EXPECTS(in, "Register", in.opA, !in.opB, !in.opC);
            out = m_INR;
            out |= encode_regpair(in.opA->value) >> 2;
            break;
        case DCX:
        EXPECTS(in, "Register", in.opA, !in.opB, !in.opC);
            out = m_INR;
            out |= encode_regpair(in.opA->value) >> 2;
            break;
        case DAD:
        EXPECTS(in, "RegisterPair", in.opA, !in.opB, !in.opC);
            out = m_DAD;
            out |= encode_regpair(in.opA->value) >> 2;
            break;
        case DAA:
        EXPECTS(in, "Nothing", !in.opA, !in.opB, !in.opC);
            out = m_DAA;
            break;
        case ANA:
        EXPECTS(in, "Register", in.opA, !in.opB, !in.opC);
            out = m_ANA;
            out |= encode_register(in.opA->value) >> 2;
            break;
        case ANI:
        EXPECTS(in, "Byte", in.opA, !in.opB, !in.opC);
            out = m_ANI;
            break;
        case ORA:
        EXPECTS(in, "Register", in.opA, !in.opB, !in.opC);
            out = m_ORA;
            out |= encode_register(in.opA->value) >> 2;
            break;
        case ORI:
        EXPECTS(in, "Byte", in.opA, !in.opB, !in.opC);
            out = m_ORI;
            break;
        case XRA:
        EXPECTS(in, "Register", in.opA, !in.opB, !in.opC);
            out = m_XRA;
            out |= encode_register(in.opA->value) >> 2;
            break;
        case XRI:
        EXPECTS(in, "Byte", in.opA, !in.opB, !in.opC);
            out = m_XRI;
            break;
        case CMP:
        EXPECTS(in, "Register", in.opA, !in.opB, !in.opC);
            out = m_CMP;
            out |= encode_register(in.opA->value) >> 5;
            break;
        case CPI:
        EXPECTS(in, "Byte", in.opA, !in.opB, !in.opC);
            out = m_ORI;
            break;
        case RLC:
        EXPECTS(in, "Nothing", !in.opA, !in.opB, !in.opC);
            out = m_RLC;
            break;
        case RRC:
        EXPECTS(in, "Nothing", !in.opA, !in.opB, !in.opC);
            out = m_RRC;
            break;
        case RAL:
        EXPECTS(in, "Nothing", !in.opA, !in.opB, !in.opC);
            out = m_RAL;
            break;
        case RAR:
        EXPECTS(in, "Nothing", !in.opA, !in.opB, !in.opC);
            out = m_RLC;
            break;
        case CMA:
        EXPECTS(in, "Nothing", !in.opA, !in.opB, !in.opC);
            out = m_CMA;
            break;
        case CMC:
        EXPECTS(in, "Nothing", !in.opA, !in.opB, !in.opC);
            out = m_CMC;
            break;
        case STC:
        EXPECTS(in, "Nothing", !in.opA, !in.opB, !in.opC);
            out = m_STC;
            break;
        case JMP:
        EXPECTS(in, "MemLo, MemHi", in.opA, in.opB, in.opC);
            out = m_JMP;
            break;
        case Jccc:
        EXPECTS(in, "MemLo, MemHi", in.opA, in.opB, in.opC);
            out = m_Jccc;
            break;
        case CALL:
        EXPECTS(in, "MemLo, MemHi", in.opA, in.opB, in.opC);
            out = m_CALL;
            break;
        case Cccc:
        EXPECTS(in, "MemLo, MemHi", in.opA, in.opB, in.opC);
            out = m_Cccc;
            break;
        case RET:
        EXPECTS(in, "Nothing", !in.opA, !in.opB, !in.opC);
            out = m_RET;
            break;
        case Rccc:
        EXPECTS(in, "Condition", in.opA, !in.opB, !in.opC);
            out = m_Rccc;
            out |= encode_cond(in.opA->value);
            break;
        case RST:
        EXPECTS(in, "Nothing", !in.opA, !in.opB, !in.opC);
            warn ("RST: i8080-emu does not implement this instruction and will panic");
            out = m_RST;
            break;
        case PCHL:
        EXPECTS(in, "Nothing", !in.opA, !in.opB, !in.opC);
            out = m_PCHL;
            break;
        case PUSH:
        EXPECTS(in, "RegPair", in.opA, !in.opB, !in.opC);
            out = m_PUSH;
            out |= encode_regpair(in.opA->value) >> 2;
            break;
        case POP:
            out = m_POP;
            out |= encode_regpair(in.opA->value) >> 2;
            break;
        case XTHL:
        EXPECTS(in, "Nothing", !in.opA, !in.opB, !in.opC);
            out = m_XTHL;
            break;
        case SPHL:
        EXPECTS(in, "Nothing", !in.opA, !in.opB, !in.opC);
            out = m_SPHL;
            break;
        case IN:
        EXPECTS(in, "Port", in.opA, !in.opB, !in.opC);
            warn("IN: i8080-emu does not implement this instruction and will panic");
            out = m_IN;
            break;
        case OUT:
        EXPECTS(in, "Port", in.opA, !in.opB, !in.opC);
            warn("IN: i8080-emu does not implement this instruction and will panic");
            out = m_OUT;
            break;
        case EI:
        EXPECTS(in, "Nothing", !in.opA, !in.opB, !in.opC);
            out = m_EI;
            break;
        case DI:
        EXPECTS(in, "Nothing", !in.opA, !in.opB, !in.opC);
            out = m_DI;
            break;
        case HLT:
        EXPECTS(in, "Nothing", !in.opA, !in.opB, !in.opC);
            out = m_HLT;
            break;
        case NOP:
        EXPECTS(in, "Nothing", !in.opA, !in.opB, !in.opC);
            out = m_NOP;
            break;
        default:
            error("Unknown instruction %d", in.type);
            exit(1);
    }

    return out;
}