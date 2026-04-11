#include <stdio.h>
#include "unit.h"
#include "encoder/encoder.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')

int main() {
    operand db = {
        .value = HL,
    };
    operand db2 = {
        .type = IMM_BYTE,
        .value = 123,
    };

    comp_unit cu = {
        .next = 0,
        .type = LXI,
        .opA = &db,
        .opB = &db2,
        .opC = &db2,
    };

    byte b = encode(cu);

    printf("Out 0b"BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(b));
}