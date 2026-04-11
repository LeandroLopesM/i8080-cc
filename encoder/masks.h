#ifndef MASKS_HPP
#define MASKS_HPP

#define DF_REG_MASK 0b00111000
#define DF_RP_MASK 0b00110000

#define m_MOV  0b01000000 // MOV D,S   01DDDSSS          -       Move register to register
#define m_MVI  0b00000110 // MVI D,#   00DDD110 db       -       Move immediate to register
#define m_LXI  0b00000001 // LXI RP,#  00RP0001 lb hb    -       Load register pair immediate
#define m_LDA  0b00111010 // LDA a     00111010 lb hb    -       Load A from memory
#define m_STA  0b00110010 // STA a     00110010 lb hb    -       Store A to memory
#define m_LHLD 0b00101010 // LHLD a    00101010 lb hb    -       Load H:L from memory
#define m_SHLD 0b00100010 // SHLD a    00100010 lb hb    -       Store H:L to memory
#define m_LDAX 0b00001010 // LDAX RP   00RP1010 *1       -       Load indirect through BC or DE
#define m_STAX 0b00000010 // STAX RP   00RP0010 *1       -       Store indirect through BC or DE
#define m_XCHG 0b11101011 // XCHG      11101011          -       Exchange DE and HL content
#define m_ADD  0b10000000 // ADD S     10000SSS          ZSPCA   Add register to A
#define m_ADI  0b11000110 // ADI #     11000110 db       ZSCPA   Add immediate to A
#define m_ADC  0b10001000 // ADC S     10001SSS          ZSCPA   Add register to A with carry
#define m_ACI  0b11001110 // ACI #     11001110 db       ZSCPA   Add immediate to A with carry
#define m_SUB  0b10010000 // SUB S     10010SSS          ZSCPA   Subtract register from A
#define m_SUI  0b11010110 // SUI #     11010110 db       ZSCPA   Subtract immediate from A
#define m_SBB  0b10011000 // SBB S     10011SSS          ZSCPA   Subtract register from A with borrow
#define m_SBI  0b11011110 // SBI #     11011110 db       ZSCPA   Subtract immediate from A with borrow
#define m_INR  0b00000100 // INR D     00DDD100          ZSPA    Increment register
#define m_DCR  0b00000101 // DCR D     00DDD101          ZSPA    Decrement register
#define m_INX  0b00000011 // INX RP    00RP0011          -       Increment register pair
#define m_DCX  0b00001011 // DCX RP    00RP1011          -       Decrement register pair
#define m_DAD  0b00001001 // DAD RP    00RP1001          C       Add register pair to HL (16 bit add)
#define m_DAA  0b00100111
#define m_ANA  0b10100000
#define m_ANI  0b11100110
#define m_ORA  0b10110000
#define m_ORI  0b11110110
#define m_XRA  0b10101000
#define m_XRI  0b11101110
#define m_CMP  0b10111000
#define m_CPI  0b11111110
#define m_RRC  0b00001111
#define m_RAL  0b00010111
#define m_RAR  0b00011111
#define m_CMA  0b00101111
#define m_CMC  0b00111111
#define m_STC  0b00110111
#define m_JMP  0b11000011
#define m_Jccc 0b11000010
#define m_CALL 0b11001101
#define m_Cccc 0b11000100
#define m_RET  0b11001001
#define m_Rccc 0b11000000
#define m_RST  0b11000111
#define m_PCHL 0b11101001
#define m_PUSH 0b11000101
#define m_POP  0b11000001
#define m_XTHL 0b11100011
#define m_SPHL 0b11111001
#define m_IN   0b11011011
#define m_OUT  0b11010011
#define m_EI   0b11111011
#define m_DI   0b11110011
#define m_HLT  0b01110110
#define m_NOP  0b00000000

#endif // MASKS_HPP