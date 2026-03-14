/* base library internal logging system */
#ifndef BASE_ERROR_H
#define BASE_ERROR_H

#define error_at(msg) \
    do \
    { \
        os_write(STDERR_FD, RED "[ERROR] ", LEN(RED "[ERROR] ")); \
        write_string(STDERR_FD, __FILE__); \
        write_string(STDERR_FD, ":"); \
        write_int(__LINE__); \
        write_string(STDERR_FD, " in "); \
        write_string(STDERR_FD, __func__); \
        write_string(STDERR_FD, ": "); \
        write_string(STDERR_FD, (msg)); \
        os_write(STDERR_FD, RESET "\n", LEN(RESET "\n")); \
    } while (0)

#define fatal(msg) \
    do \
    { \
        error_at(msg); \
        _exit(1); \
    } while (0)

#define assert_msg(expr, msg) \
    do \
    { \
        if (!(expr)) \
        { \
            fatal(msg); \
        } \
    } while (0)

#define warn(msg) \
    do \
    { \
        os_write(STDERR_FD, YELLOW "[WARN] ", LEN(YELLOW "[WARN] ")); \
        write_string(STDERR_FD, __FILE__); \
        write_string(STDERR_FD, ":"); \
        write_int(__LINE__); \
        write_string(STDERR_FD, ": "); \
        write_string(STDERR_FD, (msg)); \
        os_write(STDERR_FD, RESET "\n", LEN(RESET "\n")); \
    } while (0)

#endif /* BASE_ERROR_H */
