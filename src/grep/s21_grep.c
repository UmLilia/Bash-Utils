#include "s21_grep.h"

int main(int argc, char **argv) {
  Flags flags = {0};
  char *arg = NULL;
  get_flags(argc, argv, &flags, &arg);
  if (!arg && optind < argc && !flags.f) find_arg(argv, &arg);
  if (arg) {
    if (optind >= argc)
      get_result(stdin, flags, arg, arg, 0);
    else
      open_file(argc, argv, flags, arg);
    free(arg);
  } else
    fprintf(stderr, "error: There is no pattern\n");
  return 0;
}

void get_flags(int argc, char **argv, Flags *flags, char **arg) {
  int opt;
  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (opt) {
      case 'e':
        flags->e = 1;
        *arg = malloc(sizeof(char) * (strlen(optarg) + 1));
        if (arg)
          strcpy(*arg, optarg);
        else
          fprintf(stderr, "s21_grep: Failed to allocate memory");
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        flags->f = 1;
        arg_from_file(arg, optarg, *flags);
        break;
      case 'o':
        flags->o = 1;
        break;
      case '?':
        fprintf(stderr, "error: Unknown flag\n");
        break;
    }
  }
}

void arg_from_file(char **arg, char *filename, const Flags flags) {
  size_t n = 0;
  FILE *file = fopen(filename, "r");
  if (!file && !flags.s)
    fprintf(stderr, "error: Nonexistent or unreadable file '%s'\n", filename);
  else if (file) {
    getline(arg, &n, file);
    fclose(file);
  }
}

void find_arg(char **argv, char **arg) {
  *arg = malloc(sizeof(char) * (strlen(argv[optind]) + 1));
  if (arg) {
    strcpy(*arg, argv[optind]);
    argv[optind] = '\0';
    optind++;
  } else
    fprintf(stderr, "s21_grep: Failed to allocate memory");
}

void open_file(int argc, char **argv, const Flags flags, char *arg) {
  int is_alone = 0;
  if (optind < argc - 1 && !flags.h) is_alone = 1;
  for (; optind < argc; optind++) {
    FILE *file = fopen(argv[optind], "r");
    if (!file && !flags.s)
      fprintf(stderr, "error: Nonexistent or unreadable file '%s'\n",
              argv[optind]);
    else if (file) {
      get_result(file, flags, arg, argv[optind], is_alone);
      fclose(file);
    }
  }
}

void get_result(FILE *file, const Flags flags, char *arg, char *filename,
                int is_alone) {
  char *line = NULL, last_symbol;
  size_t n = 0;
  int line_number = 1, match_count = 0, line_count = 1;
  while (getline(&line, &n, file) != -1) {
    char *result = NULL;
    last_symbol = line[strlen(line) - 1];
    if (flags.i) {
      my_strcasestr(line, &arg, &result);
    } else
      result = strstr(line, arg);
    if ((result && !flags.v) || (!result && flags.v)) {
      match_count++;
      if (!flags.c && !flags.l && !flags.o) {
        if (is_alone == 1) printf("%s:", filename);
        if (flags.n) printf("%d:", line_number);
        printf("%s", line);
        if (last_symbol != '\n') printf("\n");
      }
      if (flags.o && !flags.v && !flags.c && !flags.l)
        get_matched_parts(result, arg, is_alone, filename, line_number, flags);
    }
    if (!result && flags.v) line_count++;
    line_number++;
  }
  if (flags.c && !flags.l) {
    if (is_alone == 1) printf("%s:", filename);
    printf("%d\n", match_count);
  }
  if (flags.l) {
    if ((!flags.v && match_count > 0) || (flags.v && match_count > 0))
      printf("%s\n", filename);
  }
  if (line) free(line);
}

void my_strcasestr(char *line, char **arg, char **result) {
  int result_point = 0;
  char *new_line = malloc(sizeof(char) * (strlen(line) + 1)),
       *new_arg = malloc(sizeof(char) * (strlen(*arg) + 1));
  if (new_arg && new_line) {
    strcpy(new_arg, *arg);
    strcpy(new_line, line);
    my_tolower(new_line);
    my_tolower(new_arg);
    *result = strstr(new_line, new_arg);
    if (*result) {
      result_point = *result - new_line;
      *result = line + result_point;
    }
    free(new_line);
    free(new_arg);
  } else
    fprintf(stderr, "s21_grep: Failed to allocate memory");
}

void my_tolower(char *line) {
  for (size_t i = 0; i < strlen(line); i++) {
    if (line[i] >= 65 && line[i] <= 90) line[i] += 32;
  }
}

void get_matched_parts(char *result, char *arg, int is_alone, char *filename,
                       int line_number, const Flags flags) {
  do {
    if (is_alone == 1) printf("%s:", filename);
    if (flags.n) printf("%d:", line_number);
    for (size_t i = 0; i < strlen(arg); i++) {
      putchar(result[i]);
    }
    putchar('\n');
    result += strlen(arg);
    if (flags.i)
      my_strcasestr(result, &arg, &result);
    else
      result = strstr(result, arg);
  } while (result != NULL);
}