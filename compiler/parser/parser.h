#ifndef PARSER_H
#define PARSER_H

#include "util/arrays.h"
#include "../unit.h"
#include "util/arrays.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    map labels;

    struct {
        enum {
            BINARY,
            EXE_8080
        } out_type;
    } flags;
} state;

string_arr* tokenize(char* c);
comp_unit parse_line(state* s, char* line, size_t current_offset, int* fail_flag);

#endif // PARSER_H