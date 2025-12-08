#include "yade/lexer.h"

struct keyword {
  const char *name;
  yade_token_type type;
};

static struct keyword keywords[] = {
    {"make", TOKEN_MAKE},     {"put", TOKEN_PUT},   {"get", TOKEN_GET},
    {"remove", TOKEN_REMOVE}, {"redo", TOKEN_REDO}, {"is", TOKEN_IS}};

// lexer_advance: move lexer state (position & current_car) forward by one
static void lexer_advance(yade_lexer *lexer) {
  lexer->position++;

  if (lexer->input[lexer->position] != '\0') {
    lexer->current_char = lexer->input[lexer->position];
  } else {
    lexer->current_char = '\0'; // endOinput
  }
}

// lexer_skip_whitespace: advance lexer if current_char is
// whitespace, tab, newline or carriage return
static void lexer_skip_whitespace(yade_lexer *lexer) {
  while (lexer->current_char == ' ' || lexer->current_char == '\t' ||
         lexer->current_char == '\n' || lexer->current_char == '\r') {
    lexer_advance(lexer);
  }
}

// lexer_init: initialize lexer state
void lexer_init(yade_lexer *lexer, const char *input) {
  lexer->input = input;
  lexer->position = 0;
  if (input[0] != '\0') {
    lexer->current_char = input[0];
  } else {
    lexer->current_char = '\0';
  }
}

// make_token: builds token struct frfr
static yade_token make_token(yade_token_type type, char *value) {
  yade_token t;
  t.type = type;
  t.value = value;
  return t;
}

// lexer_token_type_name: maps token values to names, just for easier debugging
const char *lexer_token_type_name(yade_token_type type) {
  switch (type) {
  default:
    return "UNKNOWN";
  }
}
