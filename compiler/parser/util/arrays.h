#ifndef ARRAYS_H
#define ARRAYS_H

#include <stddef.h>

typedef struct {
    size_t len;
    size_t cap;
    char* items;
} string;

typedef struct {
    size_t len;
    size_t cap;
    string* items;
} string_arr;

typedef struct {
    size_t len;
    size_t cap;
    int* items;
} int_arr;

typedef struct {
    size_t len;
    size_t cap;

    string_arr keys;
    int_arr values;
} map;

string mkstr(const char* it);
void push_string(string_arr*, string it);
void push_int(int_arr*, int it);
void push_char(string *, char);
string cp_str(string);

#endif // ARRAYS_H