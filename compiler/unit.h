#ifndef UNIT_H
#define UNIT_H

#include "../core/common.h"

enum operand_kind {
    IMM_WORD,   // X lb, hb
    IMM_BYTE,   // X b
    MEMORY,     // X [HL]
    REGISTER    // X[R]
};

#ifndef INSTRUNCTION_KIND
#define INSTRUNCTION_KIND
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
#endif // INSTRUNCTION_KIND

typedef byte operand;

enum reg {
    REG_A,
    REG_B,
    REG_C,
    REG_D,
    REG_E,
    REG_H,
    REG_L,
    REG_M
};

enum reg_pair {
    RP_BC,
    RP_DE,
    RP_HL,
    RP_SP,
    RP_PSW
};

enum reg_cond {
    RC_NZ,
    RC_Z,
    RC_NC,
    RC_C,
    RC_PO,
    RC_PE,
    RC_P,
    RC_M
};

typedef struct _comp_unit {
    enum instruction_kind type;

    operand* opA;
    operand* opB;
    operand* opC;
} comp_unit;

#endif // UNIT_H