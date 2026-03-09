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


// the lexer acts as a table builder from a csv  file
// and parsing indivudal rows and columns
// the next step would be building a the b-tree
internal token *
tokenize_csv(string8 buffer, mem_arena *arena)
{

    b32 FL = TRUE;

    if(buffer.size < 0) return NULL;
    for(s32 index = 0; buffer.data[index] != '\0'; ++index)
    {
        token *tok = PushStruct(arena, token);
        u8 point = buffer.data[index];

        s32 start   = 0;
        s32 end     = 0;

        if(is_whitespace(point))
        {
            print("csv file is invalid");
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

    return NULL;
}

internal void
strip_new_line(string8 buffer)
{

    for (u64 index = 0; index < buffer.size; index++)
	{

	}

    return;

}

internal void
read_csv(string8 buffer)
{
    // printf("\nsize:%lu\ndata %s\n", buffer.size, buffer.data);

}

internal csv_table *
parse_csv(token *tokens, csv_table *table)
{

    return NULL;
}


#endif /* ENGINE_LEXER_H */
