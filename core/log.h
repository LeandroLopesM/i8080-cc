#ifndef LOG_H
#define LOG_H

#ifdef VERBOSE
#define WHEN_VERBOSE(...) __VA_ARGS__
#else
#define WHEN_VERBOSE(...)
#endif

typedef void(*err_handler_t(int));

void error(const char* fmt, ...);
void warn(const char* fmt, ...);
void debug(const char* fmt, ...);
void panic(const char* fmt, ...);

#endif // LOG_H