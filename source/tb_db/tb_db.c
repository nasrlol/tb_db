#define BTREE_IMPLEMENTATION
#define BASE_UNITY
#include "../base/base_include.h"

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

#include "btree_impl.h"
#include "csv_decoder.h"

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

        if(is_whitespace(codepoint)) end = index; 

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

    if(count < 2) value[1] = "./test/data.csv";

    local_persist b32 running = 1;

    mem_arena *global_arena = arena_create(GiB(1));

    string8 buffer = load_file(global_arena, value[1]);

    // NOTE(nasr): the use of tables is required for tracking headers etc.
    // i think we can optimize this away in the future but for now its fine
    csv_table      *table      = PushStructZero(global_arena, csv_table);
    csv_token_list *token_list = PushStructZero(global_arena, csv_token_list);

    token_list->start_token = &nil_csv_token; 
    token_list->end_token   = &nil_csv_token;   

    csv_token *tokens = tokenize_csv(buffer, global_arena, table, token_list);
    assert_msg(tokens != NULL, "tokens are null");

    // NOTE(nasr): token_list is now populated — pass it directly, not a fresh empty list
    btree *bt = parse_csv(global_arena, token_list, table);

    print("\nDatabase Engine\n");

    for(;;)
    {
        if(running)
        {
            u8  *lbuf     = PushArray(global_arena, u8, 256);
            s32  err      = os_read(STDIN_FD, lbuf, 256);

            if(err < 0)
            {
                print("error reading from stdin");
            }

            // TODO(nasr): extract this later in the future and make a string copy function/macro
            // @params (s32 lbuf_size, string8 lbuf_stringified)
            // NOTE(nasr): use err (bytes read) not sizeof(lbuf*) — sizeof a pointer is always 8
            s32     lbuf_size       = err;
            string8 lbuf_stringified = PushString(global_arena, lbuf_size);
            {
                memcpy(lbuf_stringified.data, lbuf, lbuf_size);
                lbuf_stringified.size = lbuf_size;
            }

            query_token_list *qtl = PushStructZero(global_arena, query_token_list);


            query_tokenizer(global_arena, &lbuf_stringified, qtl);

            // TODO(nasr): dispatch qtl against bt here

            unused(bt);

            sleep(1);
        }
    }

    return 0;
}
