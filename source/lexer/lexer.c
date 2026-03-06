// the lexer acts as a table builder from a csv  file
// and parsing indivudal rows and columns
// the next step would be building a the b-tree
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

internal token *
tokenize_csv(string8 buffer, csv_table *global_table, mem_arena *arena)
{
    i32 count = 0;
    string8 **tokens = PushArray(arena, string8 *, buffer.size / 10);
    b32 first_line = 1;

    if(buffer.size < 0) return NULL;
    for(i32 index = 0;
        buffer.data[index] != '\0';
        ++index)
    {
        csv_row *row = PushStruct(arena, csv_row);
        string8 token = {0};

        u8 point = buffer.data[index];

        u8 *start = buffer.data;
        u8 *end = NULL;

        unused(row);

        switch (point)
        {
            case '\n':
                {
                    first_line = -1;
                    break;
                }
            case ',':
                {
                    end = start - 1;

                    if (first_line)
                    {
                        global_table->headers = &token;
                        ++global_table->headers;
                        break;
                    }
                    else
                    {

                        break;
                    }
                }

            default:
                {
                    printf("point: %c\n", point);
                    count++;
                    break;
                }
        }

        token = (string8){
            .data = start,
            .size = end - start,
        };

        **tokens = token;
        ++*tokens;
    }

    printf("%d", count);

    return NULL;
}
