#include "s21_cat.h"

int main(int argc, char **argv) {
  int num_line = 1;
  Flags flags = {0};
  get_flags(argc, argv, &flags);
  if (optind >= argc) {
    get_result(stdin, flags, &num_line);
  } else
    read_file(argc, argv, flags, &num_line);
  return 0;
}

void get_flags(int argc, char **argv, Flags *flags) {
  int opt;
  while ((opt = getopt_long(argc, argv, "beEnstT", longopts, NULL)) != -1) {
    switch (opt) {
      case 'b':
        flags->number_nonblank = 1;
        break;
      case 'e':
        flags->end_of_line = 1;
        flags->v = 1;
        break;
      case 'E':
        flags->end_of_line = 1;
        break;
      case 'n':
        flags->number_all_lines = 1;
        break;
      case 's':
        flags->squeeze_blank = 1;
        break;
      case 't':
        flags->show_tabs = 1;
        flags->v = 1;
        break;
      case 'T':
        flags->show_tabs = 1;
        break;
    }
  }
}

void get_result(FILE *file, const Flags flags, int *num_line) {
  char c, prev_c = '\n';
  int num = *num_line, ASCII_OFFSET = 64, blank_count = 0;
  while ((c = fgetc(file)) != EOF) {
    if (flags.squeeze_blank && prev_c == '\n' && c == '\n') {
      blank_count++;
      if (blank_count > 1) continue;
    } else
      blank_count = 0;
    if (flags.end_of_line && c == '\n') fputc('$', stdout);
    if (flags.show_tabs && c == '\t') {
      fputc('^', stdout);
      c = 'I';
    }
    if (flags.v) {
      if ((c >= 0 && c < 9) || (c > 10 && c < 32)) {
        fputc('^', stdout);
        c += ASCII_OFFSET;
      } else if (c == 127) {
        fputc('^', stdout);
        c -= ASCII_OFFSET;
      }
    }
    if ((flags.number_nonblank) && (prev_c == '\n') && (c != prev_c))
      printf("%6d\t", num++);
    if ((flags.number_all_lines) && (prev_c == '\n') &&
        !(flags.number_nonblank))
      printf("%6d\t", num++);
    putchar(c);
    prev_c = c;
    *num_line = num;
  }
}

void read_file(int argc, char **argv, const Flags flags, int *num_line) {
  FILE *file = NULL;
  for (; optind < argc; optind++) {
    file = fopen(argv[optind], "r");
    if (file != NULL) {
      get_result(file, flags, num_line);
      fclose(file);
      file = NULL;
    } else {
      fprintf(stderr, "s21_cat: %s: No such file or directory\n", argv[optind]);
    }
  }
}