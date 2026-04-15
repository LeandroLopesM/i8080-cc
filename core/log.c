#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static void __log(char* prefix, const char* fmt, va_list va)
{
    printf("%-5s: ", prefix);
    vprintf( fmt, va);
}

void error(const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    printf("\nCompilation failed! \n");
    vprintf(fmt, va);
    va_end(va);
}

void warn(const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    __log("WARN", fmt, va);
    va_end(va);
}

void debug(const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    __log("DEBUG", fmt, va);
    va_end(va);
}


static void __panic(int die, const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    printf("Compiler error!\n");
    vprintf(fmt, va);
    va_end(va);

    if (die) abort();
}

void panic(const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    __panic(1, fmt, va);
    va_end(va);
}