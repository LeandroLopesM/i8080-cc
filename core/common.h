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

#define BINPATT "%c%c%c%c%c%c%c%c"
#define BINGEN(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')

#endif // COMMON_H