#define BASE_UNITY
#include "base/base_include.h"

internal b32
is_alpha(u8 point)
{
    return ((point >= 'a' && point <= 'z') || (point >= 'A' && point <= 'Z') || (point == '_'));
}

internal b32
is_digit(u8 point)
{
    return (point >= '0' && point <= '9');
}

internal b32
is_alpha_num(u8 point)
{
    return (is_alpha(point) || is_digit(point));
}

internal b32
is_whitespace(u8 point)
{
    return (point == '\n' || point == '\r' || point == ' ' || point == '\t');
}

internal b32
is_delimiter(u8 point)
{
    return (point == ',');
}

#include "csv_reader.h"
// #include "btree.h"

typedef struct query_token query_token;
struct query_token
{
    string8      *lexeme;
    query_token  *next;
};


// takes on line of the repl input
internal query_token *
query_tokenizer(mem_arena *arena, string8 *buffer)
{
    query_token *tok = PushStruct(arena, query_token);
    unused(tok);

    for (u64 index = 0; index < buffer->size; ++index)
    {            
        u8 codepoint = buffer->data[index];

        if(codepoint == '\n' || codepoint == '\r') break;

        s32 start   = 0; 
        s32 end     = 0; 

        if(is_whitespace(codepoint))
        {
            end = index; 
        }

        s32 new_token_size = end - start;

        tok->lexeme->data = &buffer->data[index]; 
        tok->lexeme->size = new_token_size;

        tok->next = tok;
        start = index + 1;

    }

    return tok;
}

int main(int c, char **v)
{
    if(c < 2)
    { 
        print("bad file, setting default file\n");
    }
    else v[1] =  "./test/customers-10000.csv";

    local_persist b32 running = 1;

    mem_arena *global_arena = arena_create(MiB(30));
    csv_table *global_table = PushStruct(global_arena, csv_table);

    string8 buffer = load_file(v[1]);

    print("database engine in nasr");

    for(;;)
    {
        if (running)
        {
            {
                u8 line_buffer[256] = {};
                s64 codepoint = os_read(STDIN_FD, line_buffer, 256);
                unused(codepoint);
                for(s32 index = 0; index < 256; ++index)
                {
                    if(line_buffer[index] == '\n')
                    {
                        print("exiting");
                        return 0;
                    }
                    else if(line_buffer[index] == ' ')
                    {

                        print("TODO(nasr): ");
                    }

                }
            }

            {
                read_csv(buffer);
                token *tokens = tokenize_csv(buffer, global_arena);
                global_table = parse_csv(tokens, global_table);
            }

            sleep(1);
        }
    }

    return 0;
}
