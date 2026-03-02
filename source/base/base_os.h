#ifndef BASE_OS_H
#define BASE_OS_H

internal void
print(const char *str)
{
    i32 len = 0;
    while (str[len]) len++;
    write(STDOUT_FILENO, str, len);
}

internal string8
load_file(const char *path)
{
    string8 result = {0};
    struct stat sbuf = {0};
    int err = 0;
    i32 file = open(path, O_RDONLY);

    if(file)
    {

        if(file != -1)
        {
            err = fstat(file, &sbuf);
            check(err != -1);

            result.size = (u64)sbuf.st_size;

            if(result.size !=  0)
            {
                result.data = (u8 *)mmap(0,
                                         result.size,
                                         PROT_READ,
                                         MAP_PRIVATE,
                                         file,
                                         0);

                check(result.data != MAP_FAILED);
            }

            close(file);
        }
        else
        {
           // TODO(nasr): logging
        }

    }
    return result;
}

#endif /* BASE_OS_H */
