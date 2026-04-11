#include "calc.h"
#include "common.h"
#include "log.h"
#include <stdint.h>

static byte well;
#define CHK_ADD(a, b) \
   __builtin_add_overflow(a, b, &well)
#define CHK_SUB(a, b) \
   __builtin_sub_overflow(a, b, &well)

void zsp(cpu* c, byte b)
{
    c->rgf.Z = b == 0;
    c->rgf.S = (b & 1 >> 7) == 1;
    c->rgf.P = b % 2;
}

byte add_b(cpu* c, byte lhs, byte rhs)
{
    byte b = lhs + rhs;
    c->rgf.CY = CHK_ADD(lhs, rhs);
    zsp(c, b);
    return b;
}
byte add_carry_b(cpu* c, byte lhs, byte rhs)
{
    return add_b(c, add_b(c, lhs, rhs), c->rgf.CY);
}

byte sub_b(cpu* c, byte lhs, byte rhs)
{
    byte b = lhs - rhs;
    c->rgf.CY = CHK_SUB(lhs, rhs);
    zsp(c, b);
    return b;
}
byte sub_borrow_b(cpu* c, byte lhs, byte rhs)
{
    return sub_b(c, sub_b(c, lhs, rhs), c->rgf.CY);
}

word add_w(cpu* c, word lhs, word rhs)
{
    word b = lhs + rhs;
    c->rgf.CY = CHK_ADD(lhs, rhs);
    zsp(c, b);
    return b;
}
word add_carry_w(cpu* c, word lhs, word rhs)
{
    return sub_b(c, sub_b(c, lhs, rhs), c->rgf.CY);
}

word sub_w(cpu* c, word lhs, word rhs)
{
    word b = lhs - rhs;
    word well;
    c->rgf.CY = CHK_SUB(lhs, rhs);
    zsp(c, b);
    return b;
}
word sub_borrow_w(cpu* c, word lhs, word rhs)
{
    return sub_w(c, sub_w(c, lhs, rhs), c->rgf.CY);
}
// L             H
// 0 0 0 0 0 0 0 0 > CY
byte rot_l(cpu* c, byte b)
{
    byte by = b & (1);
    b <<= 1;
    b |= by;
    c->rgf.CY = by;

    return b;
}
byte rot_r(cpu* c, byte b)
{
    byte by = b & (1 >> 7);
    b >>= 1;
    b |= by << 7;
    c->rgf.CY = by;

    return b;
}

byte rot_carry_l(cpu* c, byte b)
{
    byte by = b & (1);
    b <<= 1;
    b |= c->rgf.CY;
    c->rgf.CY = by;

    return b;
}
byte rot_carry_r(cpu* c, byte b)
{
    byte by = b & (1 >> 7);
    b >>= 1;
    b |= c->rgf.CY << 7;
    c->rgf.CY = by;

    return b;
}

void cmp_b(cpu* c, byte lhs, byte rhs)
{
    sub_b(c, lhs, rhs); // sets flags for us
    c->rgf.Z = lhs == rhs;
    c->rgf.CY = lhs < rhs;
}
void cmp_w(cpu* c, word lhs, word rhs)
{
    sub_w(c, lhs, rhs); // sets flags for us
    c->rgf.Z = lhs == rhs;
    c->rgf.CY = lhs < rhs;
}

byte pop_b(cpu* c)
{
    if (c->rgf.SP + 1 > UINT8_MAX) panic_ext(c, "Stack overflow");

    return c->memory[c->rgf.SP++];
}
void push_b(cpu* c, byte in)
{
    if ((int)(c->rgf.SP - 1) < 0) panic_ext(c, "Stack underflow");

    c->memory[c->rgf.SP--] = in;
}

word pop_w(cpu* c) {
    return (t_word){
        .asbyte = {
            .lo = pop_b(c),
            .hi = pop_b(c),
        }
    }.asword;
}
void push_w(cpu* c, word in)
{
    t_word t = (t_word){.asword = in};
    push_b(c, t.asbyte.lo);
    push_b(c, t.asbyte.hi);
}

int try_condition(cpu* c, condition_kind i){
    switch (i)
    {
        case NZ:  // ('Z'ero flag not set)
            return c->rgf.Z == 1;
        case Z:   // ('Z'ero flag set)
            return c->rgf.Z == 0;
        case NC:  // ('C'arry flag not set)
            return c->rgf.C == 0;
        case C:   // ('C'arry flag set)
            return c->rgf.C == 1;
        case PO:  // ('P'arity flag not set - ODD)
            return c->rgf.P == 0;
        case PE:  // ('P'arity flag set - EVEN)
            return c->rgf.P == 1;
        case P:   // ('S'ign flag not set - POSITIVE)
            return c->rgf.S == 0;
        case M:   // ('S'ign flag set - MINUS)
            return c->rgf.P == 1;
    }

    panic("Unreachable");
    return 1;
}
