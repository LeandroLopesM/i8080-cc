#ifndef LOG_H
#define LOG_H

#include "../cpu/cpu.h"

void error(const char* fmt, ...);

void warn(const char* fmt, ...);

void debug(const char* fmt, ...);

void panic(const char* fmt, ...);
void panic_ext(cpu *c, const char* fmt, ...);

#endif // LOG_H