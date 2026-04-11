#include <stdio.h>
#include "unit.h"
#include "encoder/encoder.h"

int main() {
    operand db = {
        .type = IMM_BYTE
    };

    comp_unit cu = {
        .next = 0,
        .type = MOV,
        .opA = &db,
        .opB = 0,
        .opC = 0,
    };

    byte b = encode(cu);

    printf("%b", b);
}