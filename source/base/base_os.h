#ifndef BASE_OS_H
#define BASE_OS_H

internal void
print(const char *str)
{
    i32 len = 0;
    while (str[len]) len++;
    write(STDOUT_FILENO, str, len);
}

#define print(Format) print(Format)

#endif /* BASE_OS_H */
