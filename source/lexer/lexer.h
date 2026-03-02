#ifndef ENGINE_LEXER_H
#define ENGINE_LEXER_H

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

};


#endif /* ENGINE_LEXER_H */
