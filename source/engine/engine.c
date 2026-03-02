#define BASE_UNITY
#include "../base/base_include.h"

#include "../lexer/lexer.h"
#include "../lexer/lexer.c"

#include "../parser/parser.h"
#include "../parser/parser.c"

#include "../repl/repl.h"
#include "../repl/repl.c"

int main(int c, char **v)
{
    mem_arena *global_arena = arena_create(MiB(1));

    unused(c);
    unused(v);

    string8 buffer = PushString(global_arena, 5);
    unused(buffer);


    for (;;)
    {
        print("reading user input...");
        // TODO(nasr): design a repl system

        sleep(1);
    }

}


