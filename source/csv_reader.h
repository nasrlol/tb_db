#ifndef ENGINE_LEXER_H
#define ENGINE_LEXER_H

typedef enum csv_token_flags csv_token_flags;
enum csv_token_flags
{
    START_FL    = 1 << 1,
    END_FL      = 1 << 2,
};

typedef enum csv_token_type csv_token_type;
enum csv_token_type
{
    // first 255 tokens  for ascii characters
    TOKEN_UNDEFINED = 255,
    TOKEN_IDENTIFIER,
    TOKEN_VALUE,
};

typedef struct csv_token csv_token;
struct csv_token
{
    string8 lexeme;
    csv_token_type type;
    csv_token_flags flags;
    csv_token *next;
};

// NOTE(nasr): i dont think im going to use this.
typedef struct csv_row csv_row;
struct csv_row
{
    // array of size col_count, points into mmap buffer
    string8 *fields;
    s32      count;
};

typedef struct csv_table csv_table;
struct csv_table
{
    // first row, col names
    // all data rows
    string8  *headers;
    csv_row  *rows;
    s32       col_count;
    s32       row_count;
};


typedef struct csv_token_list csv_token_list;
struct csv_token_list
{
    csv_token *start_token;
    csv_token *end_token;

};

read_only global_variable
csv_token nil_csv_token=
{
    .lexeme = {.data = NULL, .size =0},
    .type   = (csv_token_type)0,
    .flags  = 0,
    .next   = &nil_csv_token,

};

read_only global_variable
csv_token_list nil_csv_token_list =
{
    .start_token = &nil_csv_token,
    .end_token   = &nil_csv_token,
};


read_only global_variable
csv_row  nil_csv_row =
{
    .fields     = &nil_string,
    .count      = 0,
};

read_only global_variable
csv_table nil_csv_table =
{
    .headers     = &nil_string,
    .rows        = &nil_csv_row,
    .col_count   = 0,
    .row_count   = 0,
};

#endif /* ENGINE_LEXER_H */

// the lexer acts as a table builder from a csv  file
// and parsing indivudal rows and columns
// the next step would be building a the b-tree
internal csv_token *
tokenize_csv(string8 buffer, mem_arena *arena)
{
    b32 FL = TRUE;

    if(buffer.size < 0) return NULL;

    csv_token *tok = PushStruct(arena, csv_token);

    // URGENT(nasr): segfaulting because memcpy of strring value doesnt  work dammit
    // NOPE ITS BEECAUSE WEE DONT LOAD CSV OR SOMTHING???
    for(s32 index = 0; buffer.data[index] != '\0'; ++index)
    {
        u8 point = buffer.data[index];

        s32 start   = 0;
        s32 end     = 0;

        if(is_whitespace(point))
        {
            warn("csv file is invalid, detected whitespace");
            return NULL;
        }

        switch(point)
        {
            case('\n'):
            {
                if(FL) tok->flags |= END_FL;
                break;
            }

            case(','):
            {
                end = index - 1;
                start = index + 1;
                break;
            }
            default:
            {
                break;
            }
        }

        tok->lexeme = StringCast(&buffer.data[start], end - start);
        tok->next = tok;
    }

    return tok;
}

internal void
read_csv(string8 buffer)
{
    // printf("\nsize:%lu\ndata %s\n", buffer.size, buffer.data);

}

internal b_tree *
parse_csv(mem_arena *arena, csv_token_list *ctl)
{
    b_tree *tree = PushStructZero(arena, b_tree);
    b_tree_create(arena, tree);

    //- TODO(nasr): check initizalization or something tomorrow
    {

    }
    // TODO(nasr): fix this logic tomorrow
    csv_token *ct = PushStruct(arena, csv_token);

    for (;ct != NULL; ct = ct->next)
    {
        // skip structural ctens, only index values
        if (ct->type != TOKEN_VALUE)
        {
            continue;
        }

        // NOTE(nasr): payload is the cten itself so the caller can reach
        // row/col metadata without us having to copy it
        // NOTE(nasr): heh why do we void cast again?
        b_tree_insert(arena, tree, ct->lexeme, (void *)ct);
    }

    return tree;
}
