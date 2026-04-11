#include <stdio.h>
#include "unit.h"
#include "encoder/encoder.h"

int main() {
    operand db = {
        .type = IMM_BYTE,
        .value = 123
    };

    comp_unit cu = {
        .next = 0,
        .type = MOV,
        .opA = &db,
        .opB = &db,
        .opC = 0,
    };

    byte b = encode(cu);

    printf("%b", b);
}