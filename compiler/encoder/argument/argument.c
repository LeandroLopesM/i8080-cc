#include "argument.h"

#include "../../../core/log.h"
#include <stdlib.h>

byte encode_register(enum reg r)
{
    switch(r) {
    case A: return 0b111;
    case B: return 0b000;
    case C: return 0b001;
    case D: return 0b010;
    case E: return 0b011;
    case H: return 0b100;
    case L: return 0b101;
    case M: return 0b110;
    }

    error("Invalid register selector %d", r);
    exit(1);
}

byte encode_regpair(enum reg_pair selector)
{

    switch(selector) {
    case BC: return 0b00;
    case DE: return 0b01;
    case HL: return 0b10;
    case SP: return 0b11;
    case PSW: ; // not handled here
    }

    error("Invalid register selector %d", selector);
    exit(1);
}

byte encode_rp_xx(byte selector)
{
    switch (selector) {
    case BC: return 0b00;
    case DE: return 0b01;
    }

    error("Invalid register selector %d", selector);
    exit(1);
}

byte encode_stack_rp(byte selector)
{
    switch(selector) {
    case BC: return 0b00;
    case DE: return 0b01;
    case HL: return 0b10;
    case PSW: return 0b11;
    }

    error("Invalid register selector %d", selector);
    exit(1);
}

byte encode_cond(enum reg_cond selector)
{
    switch(selector) {
    case NZ: return 0b000;
    case Z: return 0b001;
    case NC: return 0b010;
    case _C: return 0b011;
    case PO: return 0b100;
    case PE: return 0b101;
    case P: return 0b110;
    case M: return 0b111;
    }

    error("Invalid condition selector %d", selector);
    exit(1);
}
