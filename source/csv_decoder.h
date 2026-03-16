#ifndef ENGINE_LEXER_H
#define ENGINE_LEXER_H

enum csv_token_flags
{
    FL      = 1 << 2,
};

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
    csv_token *next_token;
    enum csv_token_type type;
    enum csv_token_flags flags;
};

// NOTE(nasr): i dont think im going to use this.
typedef struct csv_row csv_row;
struct csv_row
{
    // array of size col_count, points into mmap buffer
    string8 *fields;
    s32      count;
};

#if 0
typedef struct csv_lntity csv_entity;
struct csv_entity 
{
    //- not needed because we use key header mapping i think
};
#endif

typedef struct csv_header csv_header;
struct csv_header 
{
    string8  payload;
    csv_header *next_header;
};

typedef struct csv_table csv_table;
struct csv_table
{
    // first row, col names
    // all data rows
    csv_header  *header;
    s32         row_count;
    s32         header_count;
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
    .lexeme         = {.data = NULL, .size =0},
    .type           = 0,
    .flags          = 0,
    .next_token     = &nil_csv_token,

};

read_only global_variable
csv_header nil_csv_header = 
{
    .payload =  {.data = NULL, .size = 0},
    .next_header = &nil_csv_header,
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
    .header     = &nil_csv_header,
    .row_count   = 0,
};

#endif /* ENGINE_LEXER_H */

internal b32 
is_nil_csv_token(csv_token *token)
{
    return ((token == NULL) || (token == &nil_csv_token)); 
}

internal void
csv_token_list_append_token(csv_token_list *source_token_list, csv_token *source_token)
{
    source_token_list->end_token->next_token = source_token;
    source_token_list->end_token             = source_token;

}

//- concatenate 2 token lists so we can handle parsing individual rows and concatenating them to eachother
internal void
csv_token_list_concat_list(csv_token_list *destination, csv_token_list *source)
{

    csv_token *source_ct = source->start_token;
    csv_token *destination_end_ct = destination->end_token;

     for(;!is_nil_csv_token(source_ct); source_ct = source_ct->next_token)
     {
         destination_end_ct->next_token = source_ct;
     }

     destination->end_token = source_ct;
}

#if 0
internal csv_token_list *
parse_csv_row(string8 row_buffer)
{
    // csv_token_list *

}
#endif


// the lexer acts as a table builder from a csv  file
// and parsing indivudal rows and columns
// the next step would be building a the b-tree
internal csv_token *
tokenize_csv(string8 buffer, mem_arena *arena, csv_table *table, csv_token_list *token_list)
{

    unused(token_list);
    b32 finding_headers = TRUE;

    if(buffer.size < 0) return NULL;

    csv_token *tok = PushStruct(arena, csv_token);

    // URGENT(nasr): segfaulting because memcpy of strring value doesnt  work dammit
    // NOPE ITS BEECAUSE WEE DONT LOAD CSV OR SOMTHING???
    // forgot what the solution was
    // TODO(nasr): check what the problem here was
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


        if(point == '\n') 
        {
            if(finding_headers)
            {
#if 0
                string8 headers_buffer = {.data = &buffer.data[start], .size = end - start};
#endif
                finding_headers = FALSE;

                {
                    //- map new header token list to table headers
                }
            }
#if 0
            else
            {

            }
#endif


            table->row_count++;
        }
        else if(point == ',')
        {
            if (finding_headers)
            {
                table->header_count++;
            }
        }

        switch(point)
        {
            case('\n'):
                {
                    tok->flags |= FL;
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

        tok->lexeme         = StringCast(&buffer.data[start], end - start);
        tok->next_token     = tok;
    }

    return tok;
}

//- NOTE(nasr): I don't know why we are still using that dumb table but we'll remove it in the future
internal b_tree *
parse_csv(mem_arena *arena, csv_token_list *ctl, csv_table *table)
{
    b_tree *tree = PushStructZero(arena, b_tree);
    b_tree_create(arena, tree);

    // iterate over the token list while the token is not nil
    for (csv_token *ct = ctl->start_token; !is_nil_csv_token(ct); ct = ct->next_token)
    {

        //- TODO(nasr): check initizalization or something tomorrow
        {
            //- are we parsing the first line tokens?
            //- if so, do something :))
            if(ct->flags & FL) 
            {
                // TODO(nasr): replace with nil header check function
                if(table->header != &nil_csv_header || table->header == NULL)
                {
#if 0
                    // - no this should happen in the tokenization
                    table->headers->next =     
#endif
                }
                else 
                {

                }

            }
        }

        // TODO(nasr): fix this logic tomorrow
        csv_token *ct = PushStruct(arena, csv_token);
        // skip structural ctens, only index values
        if (ct->type != TOKEN_VALUE)
        {
            continue;
        }

        // NOTE(nasr): payload is the cten itself so the caller can reach
        // row/col metadata without us having to copy it
        // NOTE(nasr): heh why do we void cast again?
        b_tree_insert(tree, ct->lexeme, (void *)ct);
    }

    return tree;
}

