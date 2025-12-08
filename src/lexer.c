#include "yade/lexer.h"

struct keyword {
  const char *name;
  lk_token_type type;
};

static struct keyword keywords[] = {
    {"make", TOKEN_MAKE},     {"put", TOKEN_PUT},   {"get", TOKEN_GET},
    {"remove", TOKEN_REMOVE}, {"redo", TOKEN_REDO}, {"is", TOKEN_IS}};
