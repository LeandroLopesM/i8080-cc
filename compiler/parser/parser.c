#include "util/mappings.h"
#include "../../core/log.h"
#include <setjmp.h>
#include "parser.h"

typedef struct {
    int used;
    byte* p;
} safe_ptr;

string_arr* tokenize(char* c)
{
    string_arr* out = (string_arr*)calloc(1, sizeof(string_arr));
    memset(out->items, 0, sizeof(string) * out->len);

    if (!c) NULL;

    static string buff;
    memset(&buff, 0, sizeof(string));

    while (*c)
    {
        if (*c == ',') goto inc;
        if (*c == ';') break;

        if (*c == ' ' || *c == '\t' )
        {
            if (buff.len != 0)
            {
                push_char(&buff, '\0');
                push_string(out, cp_str(buff));
                buff.len = 0;
            }
        }
        else push_char(&buff, *c);
inc:
        c++;
    }

    if (buff.len != 0)
    {
        push_char(&buff, '\0');
        push_string(out, cp_str(buff));
        buff.len = 0;
    }

    return out;
}

void parse_num(string in, safe_ptr a, safe_ptr b, int radix, size_t maxbyte, size_t maxword)
{
    in.items[--in.len] = '\0'; // purge postfix

    if (in.len <= maxbyte)
    {
        if (a.used)
            panic("Parser passed used pointer to numlit parser (got %d)", *a.p);

        *a.p = strtol(in.items, &in.items + in.len, radix);
    }
    else if(in.len <= maxword)
    {
        if (a.used)
            panic("A: Parser passed used pointer to numlit parser (got %d)", *a.p);
        else if (b.used)
            panic("B: Parser passed used pointer to numlit parser (got %d)", *b.p);
        t_word w = (t_word){.asword = strtol(in.items, &in.items + in.len, radix)};

        *a.p = w.asbyte.lo;
        *b.p = w.asbyte.hi;
    }
    else error("Numeric literal %s too large", in.items);

    if (*a.p == 1 && in.items[0] != '1')
        error("Failed to parse numeric literal %s", in.items);
}

void parse_numlit(string in, safe_ptr a, safe_ptr b)
{
    in.len--; // stray \0 gets caught somewhere

    switch(in.items[in.len - 1])
    {
        case 'H':
        case 'h':
            parse_num(in, a, b, 16, 2, 4);
            break;
        case 'D':
        case 'd':
            parse_num(in, a, b, 10, UINT8_MAX, UINT16_MAX);
            break;
        case 'B':
        case 'b':
            parse_num(in, a, b, 2, UINT8_MAX, UINT16_MAX);
            break;
        default:
            error("Unknown postfix for integer %s (%c)", in.items, in.items[in.len - 1]);
    }
}

void str_toupper(char* c)
{
    char* copy = c;

    while(*copy)
    {
        *copy = toupper(*copy);
        copy++;
    }
}

extern jmp_buf err_sink;

comp_unit parse_line(char* line, int *fail_flag)
{
    str_toupper(line);

    string_arr* ta = tokenize(line);
    comp_unit out;
    memset(&out, 0, sizeof(comp_unit));


    *fail_flag = 0;

    debug("Tokenized: ");

    for (size_t i = 0; i < ta->len; ++i)
    {
        printf("[%s] ", ta->items[i].items);
    }

    int buffer = str_to_instr(ta->items[0].items);

    if (buffer == -1)
        goto abort;

    out.type = buffer;

    if (ta->len > 1)
    {
        out.opA = calloc(1, sizeof(byte));
        out.opB = calloc(1, sizeof(byte));
        out.opC = calloc(1, sizeof(byte));

        safe_ptr a = {0, out.opA}, b = {0, out.opB}, c = {0, out.opC};

        for (size_t i = 1; i < ta->len; ++i)
        {
            if (ta->items[i].items[0] == '$') // $RP
            {
                if (a.used && b.used)
                    error("Register must be first or second argument in string, not %dth", i);

                if (!a.used) {
                    if (ta->items[i].len - 1 == 3) // $HL
                    {
                        *a.p = str_to_rp(ta->items[i].items);
                        a.used = 1;
                    }
                    else
                    {
                        *a.p = str_to_reg(ta->items[i].items);
                        a.used = 1;
                    }
                }
                else {
                    *b.p = str_to_reg(ta->items[i].items);
                    b.used = 1;
                }
            }
            else
            {
                if (!a.used)
                {
                    parse_numlit(ta->items[i], a, b);
                    b.used = a.used = 1;
                }
                else
                {
                    if (c.used || b.used)
                        error("No instruction takes 4 arguments");
                    parse_numlit(ta->items[i], b, c);
                    c.used = b.used = 1;
                }
            }
        }

        *out.opA = *a.p;
        *out.opB = *b.p;
        *out.opC = *c.p;
    }

    debug("Got %d %d %d %d", out.type, out.opA? *out.opA : 0, out.opB? *out.opB : 0, out.opC? *out.opC : 0);
    goto end;

abort:
    *fail_flag = 1;
end:
    return out;
}