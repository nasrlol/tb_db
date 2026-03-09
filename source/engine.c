#define BASE_UNITY
#include "base/base_include.h"

#include <stdio.h>

#include "csv_parser.h"
#include "lexer.h"

#include "lexer.c"
#include "csv_parser.c"

int main(int c, char **v)
{
    if(c < 2) return -999;

    local_persist b32 running = 1;

    mem_arena *global_arena = arena_create(MiB(30));
    csv_table *global_table = PushStruct(global_arena, csv_table);

    string8 buffer = load_file(v[1]);

    print("database engine in nasr");

    for(;;)
    {
        if (running)
        {
            // TODO(nasr): check for return key
            {
                u8 line_buffer[256] = {};

                s64 codepoint = os_read(STDIN_FD, line_buffer, 256);
                unused(codepoint);

                read_csv(buffer);
                token *tokens = tokenize_csv(buffer, global_arena);
                global_table = parse_csv(tokens, global_table);
            }

            sleep(1);
        }
    }

    return 0;
}
