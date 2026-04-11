#ifndef UNIT_H
#define UNIT_H

#include "../core/common.h"

enum operand_kind {
    IMM_WORD,   // X lb, hb
    IMM_BYTE,   // X b
    MEMORY,     // X [HL]
    REGISTER    // X[R]
};

enum instruction_kind {
    MOV , MVI , LXI , LDA , STA ,
    LHLD, SHLD, LDAX, STAX, XCHG,
    ADD , ADI , ADC , ACI , SUB ,
    SUI , SBB , SBI , INR , DCR ,
    INX , DCX , DAD , DAA , ANA ,
    ANI , ORA , ORI , XRA , XRI ,
    CMP , CPI , RLC , RRC , RAL ,
    RAR , CMA , CMC , STC , JMP ,
    Jccc, CALL, Cccc, RET , Rccc,
    RST , PCHL, PUSH, POP , XTHL,
    SPHL, IN  , OUT , EI  , DI  ,
    HLT , NOP
};


typedef struct {
    enum operand_kind type;
    byte selector;
} operand;

typedef struct _comp_unit {
    struct _comp_unit* next;

    enum instruction_kind type;
    operand* opA;
    operand* opB;
    operand* opC;
} comp_unit;

#endif // UNIT_H