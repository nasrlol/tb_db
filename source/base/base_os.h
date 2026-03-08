#ifndef BASE_OS_H
#define BASE_OS_H

internal string8
load_file(const char *path)
{
    string8 result = {0};
    struct stat sbuf = {0};

    s32 file = open(path, O_RDONLY);
    if(file == -1) return result;

    if(fstat(file, &sbuf) == -1)
    {
        print("error: fstat failed");
        close(file);
        return result;
    }

    result.size = (u64)sbuf.st_size;
    if(result.size != 0)
    {
        result.data = (u8 *)mmap(0, result.size, PROT_READ, MAP_PRIVATE, file, 0);
    }

    close(file);
    return result;
}

#endif /* BASE_OS_H */
