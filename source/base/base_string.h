#ifndef BASE_STRING_H
#define BASE_STRING_H

#define _StringCast (string)
#define StringCast { .data (u8 *)(string), .size = (sizeof((string)) - 1)}
#define PushString (Arena, Size) StringCast{.data = PushArray((arena), PushStruct(u8), (Size)), .size = (u64)(Size)} 

typedef struct string string;
struct string
{
    u8 *data;
    u64 size;
};

#endif /* BASE_STRING_H */
