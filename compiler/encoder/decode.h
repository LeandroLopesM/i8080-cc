#ifndef DECODE_H
#define DECODE_H

#include "../../core/common.h"
#include "../cpu.h"
#include "instruction.h"

instruction *decode(cpu* c, int mempos, size_t tick);
void dump_decoder();

#endif // DECODE_H