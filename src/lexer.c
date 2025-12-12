#include "yade/lexer.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct keyword {
  const char *name;
  yade_token_t type;
};

static struct keyword keywords[] = {
    {"make", TOKEN_MAKE},     {"put", TOKEN_PUT},   {"get", TOKEN_GET},
    {"remove", TOKEN_REMOVE}, {"redo", TOKEN_REDO}, {"is", TOKEN_IS},
    {"in", TOKEN_IN},         {"if", TOKEN_IF},     {"not", TOKEN_NOT},
    {"and", TOKEN_AND},       {"or", TOKEN_OR}};

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

// make_token: builds token struct
static yade_token make_token(yade_token_t type, void *val) {
  yade_token t;
  t.type = type;

  if (type == TOKEN_NUMBER) {
    t.value.num = *(int *)val;
  } else {
    t.value.str = (char *)val;
  }

  return t;
}

// lexer_is_ident_start: checks if the current char is a start to any keywords
static bool lexer_is_ident_start(char c) {
  return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}
// lexer_is_ident_part: checks if current char is part of any keywords
static bool lexer_is_ident_part(char c) {
  return lexer_is_ident_start(c) || (c >= '0' && c <= '9');
}

// lexer_read_ident: given current position&char checks all availabble keywords,
// if no match returns invalid
static yade_token lexer_read_ident(yade_lexer *lexer) {
  size_t start = lexer->position;

  while (lexer_is_ident_part(lexer->current_char)) {
    lexer_advance(lexer);
  }

  size_t len = lexer->position - start;

  char *str = malloc(len + 1);
  if (!str)
    return make_token(TOKEN_INVALID, NULL);
  memcpy(str, lexer->input + start, len);
  str[len] = '\0';

  for (int i = 0; i < (int)(sizeof(keywords) / sizeof(keywords[0])); i++) {
    size_t kw_len = strlen(keywords[i].name);

    if (kw_len == len && strncmp(str, keywords[i].name, len) == 0) {
      free(str);
      return make_token(keywords[i].type, NULL);
    }
  }

  return make_token(TOKEN_IDENTIFIER, str);
}

// lexer_get_next_token: main lexing function, returns next token
yade_token lexer_get_next_token(yade_lexer *lexer) {
  lexer_skip_whitespace(lexer);

  if (lexer_is_ident_start(lexer->current_char)) {
    return lexer_read_ident(lexer);
  }

  switch (lexer->current_char) {
  case '=':
    lexer_advance(lexer);
    return make_token(TOKEN_IS, NULL);
  case '(':
    lexer_advance(lexer);
    return make_token(TOKEN_LPAREN, NULL);
  case ')':
    lexer_advance(lexer);
    return make_token(TOKEN_RPAREN, NULL);
  case ',':
    lexer_advance(lexer);
    return make_token(TOKEN_COMMA, NULL);
  case '\0':
    return make_token(TOKEN_EOF, NULL);
  default:
    return make_token(TOKEN_INVALID, NULL);
  }
}

// lexer_token_type_name: maps token values to names, just for easier debugging
const char *lexer_token_type_name(yade_token_t type) {
  switch (type) {
  case TOKEN_MAKE:
    return "make";
  case TOKEN_GET:
    return "get";
  case TOKEN_PUT:
    return "put";
  case TOKEN_REMOVE:
    return "remove";
  case TOKEN_REDO:
    return "redo";
  case TOKEN_IS:
    return "is";
  case TOKEN_IN:
    return "in";
  case TOKEN_IF:
    return "if";
  case TOKEN_NOT:
    return "not";
  case TOKEN_AND:
    return "and";
  case TOKEN_OR:
    return "or";
  case TOKEN_EOF:
    return "EOF";
  default:
    return "UNKNOWN";
  }
}
