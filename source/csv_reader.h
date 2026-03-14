#ifndef ENGINE_LEXER_H
#define ENGINE_LEXER_H

typedef enum token_flags token_flags;
enum token_flags 
{
    START_FL    = 1 << 1,
    END_FL      = 1 << 2,
};

typedef enum token_type token_type;
enum token_type
{
    // first 255 tokens  for ascii characters
    TOKEN_UNDEFINED = 255,
    TOKEN_IDENTIFIER,
    TOKEN_VALUE,
};

typedef struct token token;
struct token
{
    string8 lexeme;
    token_type type;
    token_flags flags;
    token *next;
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
internal token *
tokenize_csv(string8 buffer, mem_arena *arena)
{
    b32 FL = TRUE;

    if(buffer.size < 0) return NULL;

    token *tok = PushStruct(arena, token);

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
parse_csv(mem_arena *arena, token *tok)
{
    b_tree *tree = PushStructZero(arena, b_tree);
    b_tree_create(arena, tree);

    for (; tok != NULL; tok = tok->next)
    {
        // skip structural tokens, only index values
        if (tok->type != TOKEN_VALUE)
        {
            continue;
        }

        // NOTE(nasr): payload is the token itself so the caller can reach
        // row/col metadata without us having to copy it
        b_tree_insert(arena, tree, tok->lexeme, (void *)tok);
    }

    return tree;
}
