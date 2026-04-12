#ifndef INSTRUCTION_MAP_H
#define INSTRUCTION_MAP_H

#include "../../unit.h"

enum instruction_kind str_to_instr(char* name);
enum reg str_to_reg(char* name);
enum reg_pair str_to_rp(char* name);

#endif // INSTRUCTION_MAP_H