#ifndef ENGINE_LEXER_H
#define ENGINE_LEXER_H

typedef enum token_flags token_flags;
enum token_flags 
{
    START_FL    = 1 << 1,
    END_FL      = 1 << 2,
};


typedef enum token_type token_type;
enum token_type
{
    // first 255 tokens  for ascii characters
    TOKEN_UNDEFINED = 255,
    TOKEN_IDENTIFIER,
    TOKEN_VALUE,
};

typedef struct token token;
struct token
{
    string8 lexeme;
    token_type type;
    token_flags flags;
    token *next;
};

#endif /* ENGINE_LEXER_H */
