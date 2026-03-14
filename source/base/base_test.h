// TODO(nasr): metaprogram that takes an expected output and generates a test for that specified 
// function
/* base library testing framework */
#ifndef BASE_TEST_H
#define BASE_TEST_H

// helper macro
#define show \
    do \
    { \
        write(STDOUT_FILENO, __FILE__, sizeof(__FILE__) - 1); \
        write(STDOUT_FILENO, ":", 1); \
        write(STDOUT_FILENO, __func__, sizeof(__func__) - 1); \
        write(STDOUT_FILENO, ":", 1); \
        write_int(__LINE__); \
        write(STDOUT_FILENO, "\n", 1); \
    } while (0)

#define test(expr) \
    { \
        if ((expr) != 0) \
        { \
            write(STDERR_FILENO, "[FAILED] ", LEN("[FAILED] ")); \
            show; \
            _exit(1); \
        } \
    }

#define verify(expr) \
    { \
        if ((expr) != 0) \
        { \
            write(STDERR_FILENO, RED "[ERROR] ", LEN(RED "[ERROR] ")); \
            show; \
            write(STDERR_FILENO, RESET, LEN(RESET)); \
            _exit(1); \
        } \
        else \
        { \
            write(STDERR_FILENO, GREEN "[SUCCESS] ", LEN(GREEN "[SUCCESS] ")); \
            show; \
            write(STDERR_FILENO, RESET, LEN(RESET)); \
        } \
    }

#endif /* BASE_TEST_H */
