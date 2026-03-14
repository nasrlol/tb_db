#ifndef BASE_OS_H
#define BASE_OS_H

internal string8
load_file(mem_arena *arena, const char *path)
{
    string8 result = {0};
    struct stat sbuf = {0};

    s32 file = open(path, O_RDONLY);
    if(file == -1)
    {
        warn("fialed to open file. path could be invalid");
        return (string8){0};
    }

    if(fstat(file, &sbuf) == -1)
    {
        warn("error: fstat failed");
        close(file);
        return (string8){0};
    }


    result = PushString(arena, sbuf.st_size);

    result.size = (u64)sbuf.st_size;
    if(result.size != 0)
    {
        result.data = (u8 *)mmap(0, result.size, PROT_READ, MAP_PRIVATE, file, 0);
    }

    close(file);
    return result;
}

#endif /* BASE_OS_H */
