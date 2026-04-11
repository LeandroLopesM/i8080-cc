#include <stdio.h>
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

    parse_line("MOV A, B, C");
}