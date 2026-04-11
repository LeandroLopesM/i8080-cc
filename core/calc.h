#ifndef CALC_H
#define CALC_H

#include "common.h"
#include "../cpu/cpu.h"

byte add_b(cpu* c, byte lhs, byte rhs);
byte add_carry_b(cpu* c, byte lhs, byte rhs);

byte sub_b(cpu* c, byte lhs, byte rhs);
byte sub_borrow_b(cpu* c, byte lhs, byte rhs);

word add_w(cpu* c, word lhs, word rhs);
word add_carry_w(cpu* c, word lhs, word rhs);

word sub_w(cpu* c, word lhs, word rhs);
word sub_borrow_w(cpu* c, word lhs, word rhs);

byte rot_l(cpu* c, byte b);
byte rot_r(cpu* c, byte b);
byte rot_carry_l(cpu* c, byte b);
byte rot_carry_r(cpu* c, byte b);

void cmp_b(cpu* c, byte lhs, byte rhs);
void cmp_w(cpu* c, word lhs, word rhs);

byte pop_b(cpu* c);
void push_b(cpu* c, byte in);

word pop_w(cpu* c);
void push_w(cpu* c, word in);

int try_condition(cpu* c, condition_kind i);

#endif // CALC_H