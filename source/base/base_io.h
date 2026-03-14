#ifndef BASE_IO_H
#define BASE_IO_H

#define STDIN_FD  0
#define STDOUT_FD 1
#define STDERR_FD 2

internal s64
os_write(s32 fd, void const *buf, u64 count) 
{
    return syscall(SYS_write, fd, buf, count);
}

internal s64
os_read(s32 fd, void *buf, u64 count)
{
    return syscall(SYS_read, fd, buf, count);
}

internal void
print_s8(string8 s)
{
    os_write(STDOUT_FILENO, s.data, s.size);
}

internal void
print(const char *str)
{
    s32 len = 0;
    while (str[len]) len++;
    os_write(STDOUT_FILENO, str, len);

}

internal void
write_int(s32 num)
{

    if (num < 0)
    {
        write(STDERR_FILENO, "-", 1);
        num = -num;
    }
    if (num >= 10)
        write_int(num / 10);
    char digit = '0' + (num % 10);

    write(STDERR_FILENO, &digit, 1);
}

internal void
write_string(s32 fd, const char *str)
{
    s32 len = 0;
    while (str[len]) len++;
    os_write(fd, str, len);
}


#endif /* BASE_IO_H */
