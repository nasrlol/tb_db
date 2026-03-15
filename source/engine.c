


#define B_TREE_IMPLEMENTATION
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

#include "b_tree.h"
#include "csv_reader.h"

typedef struct query_token query_token;
struct query_token
{
    string8      lexeme;
    query_token  *next;
};

typedef struct query_token_list query_token_list;
struct query_token_list
{
    query_token *start_token;
    query_token *current_token;
};

read_only global_variable
query_token nil_query_token =
{
    .lexeme = {.data = NULL, .size = 0},
    .next = &nil_query_token
};


read_only global_variable
query_token_list nil_query_token_list =
{
    .start_token = &nil_query_token,
    .current_token = &nil_query_token,
};

internal b32
is_nil_query_token(query_token *token)
{
    return (token == &nil_query_token) || (token == NULL);
}

internal b32
is_nil_query_token_list(query_token *token)
{
    return (token == &nil_query_token) || (token == NULL);
}


// takes on line of the repl input
// return a reference to the passed list
internal query_token_list *
query_tokenizer(mem_arena *arena, string8 *buffer, query_token_list *list)
{
    b32 initialized = 0;
    unused(initialized);

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

        // save the token
        // TODO(nasr): work on the string macros cuz no work
        {
            query_token *new_token = PushStruct(arena, query_token);

            //- initialize list
            {
                if(is_nil_query_token(list->start_token))
                {
                    list->start_token   = new_token;
                    list->current_token = new_token; 
                }
                else 
                {
                    //- all we need to do - we dont track parents or what ever. this is a token stream not a tree
                    list->current_token->next = new_token;
                }
            }

            s32 new_token_size = end - start;

            new_token->lexeme = PushString(arena, new_token_size);
            new_token->lexeme.data = &buffer->data[index]; 
            new_token->lexeme.size = new_token_size;

            list->current_token->next = new_token;

            start = index + 1;
        }
    }

    return list;
}

int main(int count, char **value)
{

#if 1
    unused(nil_query_token_list);
#endif

    if(count < 2) value[1] =  "./test/data.csv";

    local_persist b32 running = 1;

    mem_arena *global_arena = arena_create(MiB(30));

    // NOTE(nasr): see note down below
    // csv_table *global_table = PushStruct(global_arena, csv_table);

    string8 buffer = load_file(global_arena, value[1]);

    print("\nDatabase Engine\n");

    for(;;)
    {
        if (running)
        {
            {
                u8 *lbuf = PushArray(global_arena, u8, 256);
                s32 err = os_read(STDIN_FD, lbuf, 256);

                if(err < 0)
                {
                    print("error reading from stdin");
                }

                // TODO(nasr): extract this later in the future and make a string copy function/macro
                // @params (s32 lbuf_size , string8 lbuf_stringified)
                s32 lbuf_size             = sizeof(lbuf) - 1;
                string8 lbuf_stringified  = PushString(global_arena, lbuf_size);
                {
                    memcpy(lbuf_stringified.data, lbuf, lbuf_size);
                    lbuf_stringified.size  = sizeof(lbuf) - 1;
                }

                query_token_list *qtl = PushStruct(global_arena, query_token_list);

                query_tokenizer(global_arena, &lbuf_stringified, qtl);
            }

            {
                read_csv(buffer);

                csv_token *tokens = tokenize_csv(buffer, global_arena);

                assert_msg(tokens != NULL, "Tokens are NULL.");

                csv_token_list *ctl = PushStruct(global_arena, csv_token_list);
                b_tree *bt = parse_csv(global_arena, ctl);

                b_tree_write(bt);
            }


            // NOTE(nasr): not sure on how to approach the b-tree and the  table format thing
            // we kind of want our table format i think? but i wouldnt be sure about the use case
            // so we stick to the regular b_tree for now. commenting out the tables.

            sleep(1);
        }
    }

    return 0;
}
