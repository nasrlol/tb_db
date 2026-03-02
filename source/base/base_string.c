internal b8
compare_string(string c1, string c2)
{
    if (c1.size != c2.size)
    {
        return 0;
    }

    for (u64 index = 0; index < c1.size; ++index)
    {
        if (c1.data[index] != c2.data[index])
        {
            return 0;
        }
    }

    return 1;
}

internal b8
compare_u8(u8 *c1, u8 *c2, u64 len)
{
    if (sizeof(c1) != len || sizeof(c2) != len) 
    {
        return 0;
    }

    if (sizeof(*c1) != sizeof(*c2))
    {
        return 0;
    }

    for (u64 word_idx = 0; word_idx <= sizeof(*c1); ++word_idx)
    {
        if (*c1 != *c2)
        {
            return 0;
        }

        ++c1;
        ++c2;
    }

    return 1;
}

internal u64
to_u64(u8 *buf, umm len)
{
    u64 value = 0;

    for (umm buffer_idx = 0; buffer_idx < len; ++buffer_idx)
    {
        u8 c = buf[buffer_idx];
        if (c < '0' || c > '9')
        {
            break;
        }
        value = value * 10 + (c - '0');
    }

    return value;
}
