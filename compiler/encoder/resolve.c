#include "../../core/log.h"
#include "resolve.h"

byte* resolve(cpu* c, byte selector)
{
    switch(selector) {
    case 0b111: return &c->rgf.A;
    case 0b000: return &c->rgf.B;
    case 0b001: return &c->rgf.C;
    case 0b010: return &c->rgf.D;
    case 0b011: return &c->rgf.E;
    case 0b100: return &c->rgf.H;
    case 0b101: return &c->rgf.L;
    case 0b110: return &c->memory[c->rgf.HL];
    }

    panic_ext(c, "Invalid register selector %#x", selector);
    return 00; // unreachable
}

byte* resolve_rp(cpu* c, byte selector)
{

    switch(selector) {
    case 0b00: return (byte*)&c->rgf.BC;
    case 0b01: return (byte*)&c->rgf.DE;
    case 0b10: return (byte*)&c->rgf.HL;
    case 0b11: return (byte*)&c->rgf.SP; //   (Stack pointer, refers to PSW (FLAGS:A) for PUSH/POP)
    }

    panic_ext(c, "Invalid register selector %#x", selector);
    return 00; // unreachable
}

byte* resolve_rp_xx(cpu* c, byte selector)
{
    switch (selector) {
    case 0b00: return (byte*)&c->rgf.BC;
    case 0b01: return (byte*)&c->rgf.DE;
    }

    panic_ext(c, "Invalid register selector %#x", selector);
    return 00; // unreachable
}

byte* resolve_stack_rp(cpu* c, byte selector)
{
    switch(selector) {
    case 0b00: return (byte*)&c->rgf.BC;
    case 0b01: return (byte*)&c->rgf.DE;
    case 0b10: return (byte*)&c->rgf.HL;
    case 0b11: return (byte*)&c->rgf.PSW; //   (Stack pointer, refers to PSW (FLAGS:A) for PUSH/POP)
    }

    panic_ext(c, "Invalid register selector %#x", selector);
    return 00; // unreachable
}

condition_kind* resolve_condition(byte selector)
{
    static condition_kind buffer = NZ;

    switch(selector) {
    case 0b000: buffer = NZ; break; // ('Z'ero flag not set)
    case 0b001: buffer = Z;  break;  // ('Z'ero flag set)
    case 0b010: buffer = NC; break; // ('C'arry flag not set)
    case 0b011: buffer = C;  break;  // ('C'arry flag set)
    case 0b100: buffer = PO; break; // ('P'arity flag not set - ODD)
    case 0b101: buffer = PE; break; // ('P'arity flag set - EVEN)
    case 0b110: buffer = P;  break;  // ('S'ign flag not set - POSITIVE)
    case 0b111: buffer = M;  break;  // ('S'ign flag set - MINUS)
    }

    return &buffer;
}
