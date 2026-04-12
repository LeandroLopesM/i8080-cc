#include <stdio.h>
#include "../core/log.h"
#include "parser/parser.h"
#include "unit.h"
#include "encoder/encoder.h"

int main() {
    // operand db = HL;
    // operand db2 = 123;

    // comp_unit cu = {
    //     .next = 0,
    //     .type = LXI,
    //     .opA = &db,
    //     .opB = &db2,
    //     .opC = &db2,
    // };

    // byte b = encode(cu);

    // printf("Out 0b"BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(b));

    state s;
    comp_unit cu = parse_line(&s, "LXI $DE, 123h", 0);
    printf("I: %d\n", cu.type);
    printf("A: %d\n", cu.opA? *cu.opA : 0);
    printf("B: %d\n", cu.opB? *cu.opB : 0);
    printf("C: %d\n", cu.opC? *cu.opC : 0);

    printf("%#X", encode(cu));
}