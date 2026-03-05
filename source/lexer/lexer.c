internal b32
is_alpha(u8 point)
{
  return ((point >= 'a' && point <= 'z') ||
          (point >= 'A' && point <= 'Z') ||
          (point == '_'));
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
  return (point == '\n' || point == '\r' ||
          point == ' ' || point == '\t');
}

internal b32
is_delimiter(u8 point)
{

    return (point == ',');

}

internal token *
tokenize_csv(string8 buffer)
{
    i32 count = 0;
    string8 **tokens = PushArray(arena, string8 *, buffer.size / 10);

    if(buffer.size < 0) return NULL;
    for(i32 index = 0;
         buffer.data[index] != '\0';
         ++index)
    {
        string8 tokens = {0};

        u8 point = buffer.data[index];
        if(is_whitespace(point)) continue;

        u8 *start = &buffer.data;

        if(is_delimiter(point))
        {


        }

        u8 *end = start - 1;

        unused(start);
        unused(end);

        switch (point)
        {

            default:
                {
                    printf("point: %c\n", point);
                    count++;
                }
        }

    }

    printf("%d", count);

    return NULL;
}
