#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>

typedef struct Flags {
  int number_nonblank, end_of_line, number_all_lines, squeeze_blank, show_tabs,
      v;
} Flags;

static struct option longopts[] = {{"number-nonblank", no_argument, NULL, 'b'},
                                   {"number", no_argument, NULL, 'n'},
                                   {"squeeze-blank", no_argument, NULL, 's'},
                                   {NULL, 0, NULL, 0}};

void get_flags(int argc, char **argv, Flags *flags);
void get_result(FILE *file, const Flags flags, int *num_line);
void read_file(int argc, char **argv, const Flags flags, int *num_line);

#endif