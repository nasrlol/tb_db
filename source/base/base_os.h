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

internal string8
write_file(const char *path, string8 data)
{

    string8 result = {0};
    s32 file = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(file == -1)
    {
        warn("failed to open file for writing. path could be invalid");
        return (string8){0};
    }
    
    u64 written = 0;
    while(written < data.size)
    {
        s64 err = write(file, data.data + written, data.size - written);
        if(err == -1)
        {
            warn("write syscall failed");
            close(file);
            return (string8){0};
        }
        written += err;
    }
    
    close(file);
    result = data;
    return result;
}

#endif /* BASE_OS_H */
