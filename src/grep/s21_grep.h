#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Flags {
  int e, i, v, c, l, n, h, s, f, o;
} Flags;

void get_flags(int argc, char **argv, Flags *flags, char **arg);
void arg_from_file(char **arg, char *filename, const Flags flags);
void find_arg(char **argv, char **arg);
void open_file(int argc, char **argv, const Flags flags, char *arg);
void get_result(FILE *file, const Flags flags, char *arg, char *filename,
                int is_alone);
void my_strcasestr(char *line, char **arg, char **result);
void my_tolower(char *line);
void get_matched_parts(char *result, char *arg, int is_alone, char *filename,
                       int line_number, const Flags flags);

#endif