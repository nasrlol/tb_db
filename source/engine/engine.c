#define BASE_UNITY
#include "../base/base_include.h"

#include <stdio.h>

#include "../lexer/lexer.h"
#include "../lexer/lexer.c"

#include "../parser/parser.h"
#include "../parser/parser.c"

#include "../repl/repl.h"
#include "../repl/repl.c"

#include "../storage/csv_reader.h"
#include "../storage/csv_reader.c"


int main(int c, char **v)
{
    if(c < 2) return -999;

    string8 buffer = load_file(v[1]);
    // read_csv(buffer);
    tokenize_csv(buffer);


    // for(;;)
    // {
    //     print("reading user input...");
    //     // TODO(nasr): design a repl system
    //
    //     sleep(1);
    // }
    //

    return 0;
}


