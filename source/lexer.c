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

        tok->lexeme = String8Cast(&buffer.data[start], end - start);
        tok->next = tok;
    }

    return NULL;
}
