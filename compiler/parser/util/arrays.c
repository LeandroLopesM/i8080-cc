#include "arrays.h"
#include <stdlib.h>
#include <string.h>

#define DA_APPEND(arrname, typename)                                \
void push_##typename (arrname *da, typename add) {                  \
    if (da->len + 1 >= da->cap)                                     \
    {                                                               \
        if (da->cap == 0) da->cap = 128;                            \
        else da->cap *= 2;                                          \
        da->items = realloc(da->items, sizeof(typename) * da->cap); \
    }                                                               \
    da->items[da->len++] = add;                                     \
}                                                                   \


DA_APPEND(string_arr, string)
DA_APPEND(int_arr, int)
DA_APPEND(string, char)

string mkstr(const char *it)
{
    string tmp;
    memset(&tmp, 0, sizeof(string));

    if(!it) return tmp;

    tmp.len = strlen(it);
    tmp.cap = strlen(it) + 1;
    memcpy(&tmp, it, tmp.len);

    return tmp;
}

string cp_str(string in)
{
    string tmp = {
        .len = in.len,
        .cap = in.len + 1,
    };

    tmp.items = malloc(tmp.cap);

    memcpy(tmp.items, in.items, in.len);
    return tmp;
}