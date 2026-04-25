// single header csv decoder implementation
#ifndef CSV_DECODER_H
#define CSV_DECODER_H

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
    b32         finding_headers;
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
    .lexeme         = {.data = NULL, .size = 0},
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

#ifdef CSV_IMPLEMENTATION
internal b32
is_nil_csv_token(csv_token *token)
{
    return ((token == NULL) || (token == &nil_csv_token));
}


#if 1
internal void
csv_token_list_append_token(csv_token_list *source_token_list, csv_token *source_token)
{
    source_token->next_token = NULL;

    if(source_token_list->start_token == NULL) {
        source_token_list->start_token = source_token;
        source_token_list->end_token   = source_token;
    } else {
        source_token_list->end_token->next_token = source_token;
        source_token_list->end_token             = source_token;
    }
}
#else

// TODO(nasr): segfaulting because end_token not allocated
internal void
csv_token_list_append_token(csv_token_list *source_token_list, csv_token *source_token)
{
    source_token_list->end_token->next_token = source_token;
    source_token_list->end_token             = source_token;
}
#endif

//- concatenate 2 token lists so we can handle parsing individual rows and concatenating them to eachother
internal void
csv_token_list_concat_list(csv_token_list *destination, csv_token_list *source)
{
    if(is_nil_csv_token(source->start_token)) return;

    csv_token *source_ct      = source->start_token;
    csv_token *destination_et = destination->end_token;

    // walk source and stitch each node onto destination's tail
    for(; !is_nil_csv_token(source_ct); source_ct = source_ct->next_token)
    {
        destination_et->next_token = source_ct;
        destination_et             = source_ct;
    }

    // destination_et now points at the last real source node (not the nil sentinel)
    destination->end_token = destination_et;
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

    assert_msg(buffer.size != 0, "[AFTER INITIALIZATION] buffer size is 0");

    // URGENT(nasr): segfaulting because memcpy of strring value doesnt  work dammit
    // NOPE ITS BEECAUSE WEE DONT LOAD CSV OR SOMTHING???
    // forgot what the solution was
    // TODO(nasr): check what the problem here was

    // string size tracking across the loop not inside it
    s32 start = 0;

    for(s32 index = 0; buffer.data[index] != '\0'; ++index)
    {
        u8 point = buffer.data[index];

#if 0
        if(is_whitespace(point))
        {
            warn("csv file is invalid, detected whitespace");
            return NULL;
        }
#endif

        if(point == ',')
        {
            // emit a token for the field that ended before this comma
            csv_token *token  = PushStruct(arena, csv_token);
            token->next_token = &nil_csv_token;

            assert_msg(token != NULL, "did the push struct fail??");
            assert_msg(arena->current_position < arena->capacity, "no more arena size");

            token->lexeme     = String8(&buffer.data[start], index - start);
            token->type       = TOKEN_VALUE;
            token->next_token = &nil_csv_token;
            csv_token_list_append_token(token_list, token);

            start = index + 1;

            if(table->finding_headers)
            {
                table->header_count++;
            }
        }
        else if(point == '\n')
        {
            // emit a token for the field that ended at this newline
            csv_token *token  = PushStructZero(arena, csv_token);
            token->lexeme     = String8(&buffer.data[start], index - start);
            token->type       = TOKEN_VALUE;
            token->flags     |= FL;
            token->next_token = &nil_csv_token;

            assert_msg(token_list, "token list invalid");
            assert_msg(token, "you're tring to append an invalid token");

            csv_token_list_append_token(token_list, token);

            start = index + 1;

            if(table->finding_headers)
            {
                {
                    //- map new header token list to table headers
                }
                table->finding_headers = False;
            }

            table->row_count++;
        }
    }

    // NOTE(nasr): return the first token the caller can walk the list from token_list
    return token_list->start_token;
}

//- NOTE(nasr): I don't know why we are still using that dumb table but we'll remove it in the future
internal btree *
btree_parse_csv(mem_arena *arena, csv_token_list *ctl, csv_table *table)
{
    btree *tree = PushStructZero(arena, btree);

    s32 col_index = 0;
    s32 row_index = 0;
#ifdef CSV_DEBUG
    s32 insert_count = 0;
#endif

    // iterate over the token list while the token is not nil
    for (csv_token *ct = ctl->start_token; ct != NULL ; ct = ct->next_token)
    {
        {
            //- are we parsing the first line tokens?
            //- if so, do something :))
            if(ct->flags & FL)
            {
                // NOTE(nasr): FL marks end-of-line; advance row, reset col
                row_index++;
                col_index = 0;

                // TODO(nasr): replace with nil header check function
                // NOTE(nasr): == nil means header hasn't been set yet
                if(table->header == &nil_csv_header || table->header == NULL)
                {
#if 0
                    // - no this should happen in the tokenization
                    table->headers->next =
#endif
                }
                else
                {
                    // TODO(nasr): logging i think. came back after a while and forgot the progress on the csv decoder. this looked empty and dont file like reading the code
                    //
                }

                // FL tokens are structural, no value to index
                continue;
            }
        }

        // skip non-value tokens, only index actual cell values
        if (ct->type != TOKEN_VALUE)
        {
            col_index++;
            continue;
        }

        // NOTE(nasr): payload is the cten itself so the caller can reach
        // row/col metadata without us having to copy it
        key k = {
            .header_index = col_index,
            .row_index    = row_index,
        };

        btree_insert(arena, tree, k, (void *)ct);
        ++col_index;

#ifdef CSV_DEBUG
        ++insert_count;
        _log("Inserted %d keys, root=%p, root->key_count=%d\n",
                insert_count, tree->root, tree->root ? tree->root->key_count : -1);
#endif

    }
    return tree;
}
#endif
