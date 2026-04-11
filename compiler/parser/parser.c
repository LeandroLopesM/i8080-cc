#include "../../core/log.h"
#include "parser.h"

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

void add_label(state* s, const char* lbl, size_t coffset)
{
    if (s->flags.out_type != EXE_8080)
    {
        error("Labels are only supported by the 8080Exe format");
        exit(1);
    }

    debug("New label %s[%llu]", lbl, coffset);

    if (s->labels.len != s->labels.keys.len || s->labels.len != s->labels.values.len)
    {
        error("Map incoherent (Memory corruption?)\nQuitting to avoid mistakes");
        exit(1);
    }

    push_string(&s->labels.keys, mkstr(lbl));
    push_int(&s->labels.values, coffset);
}

comp_unit parse_line(state* s, char* line, size_t current_offset)
{
    string_arr* ta = tokenize(line);

    for (size_t i = 0; i < ta->len; ++i)
    {
        printf(" [%s] ", ta->items[i].items);
    }

    printf("\n");

    if (ta->len == 1 && ta->items[0].items[ta->items[0].len - 1] == ':')
    {
        add_label(s, ta->items[0].items, current_offset);
        return (comp_unit){0};
    }

    // str_to_instr(ta->items[0]);
}