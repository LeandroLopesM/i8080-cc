#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
typedef uint8_t byte;
typedef uint16_t word;

typedef union {
    struct {
        byte lo;
        byte hi;
    } asbyte;
    word asword;
} t_word;

#endif // COMMON_H