#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "../../core/common.h"

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
    enum instruction_kind kind;
    byte opcode;
    byte* opA;
    byte* opB;
} instruction;

#endif // INSTRUCTION_H