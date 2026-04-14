#include "argument.h"

#include "../../../core/log.h"
#include <stdlib.h>

byte encode_register(enum reg r)
{
    switch(r) {
    case REG_A: return 0b111;
    case REG_B: return 0b000;
    case REG_C: return 0b001;
    case REG_D: return 0b010;
    case REG_E: return 0b011;
    case REG_H: return 0b100;
    case REG_L: return 0b101;
    case REG_M: return 0b110;
    }

    error("Invalid register selector %d", r);
    exit(1);
}

byte encode_regpair(enum reg_pair selector)
{

    switch(selector) {
    case RP_BC: return 0b00;
    case RP_DE: return 0b01;
    case RP_HL: return 0b10;
    case RP_SP: return 0b11;
    case RP_PSW: ; // not handled here
    }

    error("Invalid register selector %d", selector);
    exit(1);
}

byte encode_rp_xx(byte selector)
{
    switch (selector) {
    case RP_BC: return 0b00;
    case RP_DE: return 0b01;
    }

    error("Invalid register selector %d", selector);
    exit(1);
}

byte encode_stack_rp(byte selector)
{
    switch(selector) {
    case RP_BC: return 0b00;
    case RP_DE: return 0b01;
    case RP_HL: return 0b10;
    case RP_PSW: return 0b11;
    }

    error("Invalid register selector %d", selector);
    exit(1);
}

byte encode_cond(enum reg_cond selector)
{
    switch(selector) {
    case RC_NZ: return 0b000;
    case RC_Z: return 0b001;
    case RC_NC: return 0b010;
    case RC_C: return 0b011;
    case RC_PO: return 0b100;
    case RC_PE: return 0b101;
    case RC_P: return 0b110;
    case RC_M: return 0b111;
    }

    error("Invalid condition selector %d", selector);
    exit(1);
}
