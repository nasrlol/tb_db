#define BASE_UNITY
#include "../base/base_include.h"

#include <stdio.h>



#include "../parser/parser.h"
#include "../parser/parser.c"

#include "../repl/repl.h"
#include "../repl/repl.c"

#include "../storage/csv_reader.h"
#include "../storage/csv_reader.c"

#include "../lexer/lexer.h"
#include "../lexer/lexer.c"



int main(int c, char **v)
{
    if(c < 2) return -999;

    mem_arena *global_arena = arena_create(MiB(20));
    csv_table *global_table = PushStruct(global_arena, csv_table);

    string8 buffer = load_file(v[1]);
    read_csv(buffer);
    tokenize_csv(buffer, global_table, global_arena);


    return 0;
}


