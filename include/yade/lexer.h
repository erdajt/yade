#ifndef YADE_LEXER_H
#define YADE_LEXER_H
#include <stddef.h>

typedef struct {
  const char *input;
  size_t position;
  char current_char;
} yade_lexer;

typedef enum {
    TOKEN_MAKE,         // make (CREATE)
    TOKEN_GET,          // get (SELECT)
    TOKEN_PUT,          // put (INSERT)
    TOKEN_REMOVE,       // remove (DELETE)
    TOKEN_REDO,         // redo (UPDATE)
    TOKEN_IS,           // is (=)
    TOKEN_IDENTIFIER,   // (table, column) names || values
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_COMMA,        // ,
    TOKEN_INVALID,
    TOKEN_NUMBER,
    TOKEN_EOF
} yade_token_t;

typedef struct {
    yade_token_t type;
      union {
        char *str;   // keywords
        int num;     // integers
    } value;
} yade_token;

void lexer_init(yade_lexer *lexer, const char *input);
const char *lexer_token_type_name(yade_token_t type);

#endif // YADE_LEXER_H
