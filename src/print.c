#include <stdarg.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

const char *banner = "      ▌\n"
                     " ▌▌▀▌▛▌█▌\n"
                     " ▙▌█▌▙▌▙▖\n"
                     " ▄▌yet another database engine\n";

#define COLOR_PURPLE "\033[95m"
#define COLOR_WHITE "\033[37m"
#define COLOR_RED "\033[91m"
#define COLOR_GREEN "\033[92m"
#define COLOR_RESET "\033[0m"

void print_banner(void) {
  printf(COLOR_GREEN "\n%s\n" COLOR_RESET, banner);
  // printf(COLOR_GREEN "yet another database engine\n\n" COLOR_RESET);
}

void print_error(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  printf(COLOR_RED);
  vprintf(fmt, args);
  printf(COLOR_RESET);

  va_end(args);
}

void print_help(void) {
  print_banner();
  printf("yade usage:\n");
  printf("\tOptions:\n");
  printf("\t  -h            display this menu\n");
}
