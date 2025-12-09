#include <stdio.h>
#include <string.h>

#include "yade/lexer.h"
#include "yade/print.h"

int main(int argc, char *argv[]) {
  const char *arg1 = argv[1];
  if (argc < 2 || (strcmp(arg1, "-h") == 0)) {
    print_help();
    return 0;
  }

  print_banner();

  char *code;

  if (strcmp(arg1, "-i") == 0 || strcmp(arg1, "--inline") == 0) {
    if (argc < 3) {
      print_error("yade: missing code after %s\n", arg1);
      return 1;
    }
    code = argv[2];
  }

  yade_lexer lexer;
  lexer_init(&lexer, code); // NOLINT

  yade_token token;
  for (;;) {
    token = lexer_get_next_token(&lexer);

    if (token.type == TOKEN_INVALID) {
      print_error("Invalid token: %c at position: %zu\n", lexer.current_char,
                  lexer.position);
      return 1;
    }

    printf("token name: %s, token type: %d\n",
           lexer_token_type_name(token.type), token.type);

    if (token.type == TOKEN_EOF) {
      break;
    }

    // printf(*expr);
  }

  return 0;
}
