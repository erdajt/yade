#include "print.h"
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc < 2 || strcmp(argv[1], "-h")) {
    print_help();
    return 0;
  }

  print_banner();

  print_error("Invalid Arguments");
  return 0;
}
