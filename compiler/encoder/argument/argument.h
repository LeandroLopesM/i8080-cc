#ifndef ARGUMENT_H
#define ARGUMENT_H

#include "../../../core/common.h"
#include "../../unit.h"

byte encode_register(enum reg r);
byte encode_regpair(enum reg_pair selector);
byte encode_rp_xx(byte selector);
byte encode_stack_rp(byte selector);
byte encode_cond(enum reg_cond selector);


#endif // ARGUMENT_H