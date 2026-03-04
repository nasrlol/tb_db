#define STD_TEST
#if defined(STD_TEST)
#include <stdio.h>
#endif

internal void
read_csv(string8 buffer)
{
#if defined(STD_TEST)
    printf("\nsize:%lu\ndata %s\n", buffer.size, buffer.data);
#endif

}


