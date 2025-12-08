#ifndef YADE_LEXER_H
#define YADE_LEXER_H
#include <stddef.h>

typedef enum {
    TOKEN_MAKE,         // make (CREATE)
    TOKEN_GET,          // get (SELECT)
    TOKEN_PUT,          // put (INSERT)
    TOKEN_REMOVE,       // remove (DELETE)
    TOKNE_REDO,         // redo (UPDATE)
    TOKEN_IS,           // is (=)
    TOKEN_IDENTIFIER,   // (table, column) names || values
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_COMMA,        // ,
    TOKEN_EOF
} lk_token_type;

typedef struct {
    lk_token_type type;
    char *value;
} lk_token;

#endif // YADE_LEXER_H
