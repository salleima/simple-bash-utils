#include "cat.h"

void get_options(int argc, char **argv, options *options) {
  static struct option long_opt[] = {{"number-nonblank", 0, NULL, 'b'},
                                     {"number", 0, NULL, 'n'},
                                     {"squeeze-blank", 0, NULL, 's'},
                                     {0, 0, NULL, 0}};

  int opt;

  while ((opt = getopt_long(argc, argv, "+benstvET", long_opt, NULL)) != -1) {
    switch (opt) {
      case 'b':
        options->b = 1;
        break;
      case 'e':
        options->e = 1;
        options->v = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 't':
        options->t = 1;
        options->v = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'E':
        options->e = 1;
        break;
      case 'T':
        options->t = 1;
        break;
      case '?':
        fprintf(stdout, "usage: cat -[benstv] [file ...]\n");
        exit(1);
    }
  }
}

void show_non_printing(int ch, options *options) {
  if (ch == '\n') {
      if (options->e)
        fprintf(stdout, "$\n");
      else
        fprintf(stdout, "%c", ch);
    } else if (ch == '\t') {
      if (options->t)
        fprintf(stdout, "^I");
      else
        fprintf(stdout, "%c", ch);
    } else if (options->v) {
      if (ch >= 32) {
        if (ch == 127) {
          fprintf(stdout, "^?");
        } else if (ch < 127) {
          fprintf(stdout, "%c", ch);
        } else if (ch < 128 + 127) {
          if (ch < 128 + 32) {
            fprintf(stdout, "M-^%c", ch - 128 + 64);
          } else {
            fprintf(stdout, "M-%c", ch - 128);
          }
        } else {
          fprintf(stdout, "M-^?");
        }
      } else {
        fprintf(stdout, "^%c", ch + 64);
      }
    } else {
      fprintf(stdout, "%c", ch);
    }
}

void cat(char *src, int file_size, options *options) {
  int ch, prev;
  char *ptr = src;
  int line_number, blank;

  line_number = 1;
  blank = 0;
  prev = '\n';

  int cur_size = 0;

  while (cur_size != file_size) {
    ch = *ptr;
    cur_size++;
    ptr++;

    if (prev == '\n') {
      if (options->s) {
        if (ch == '\n') {
          if (blank) continue;
          blank = 1;
        } else {
          blank = 0;
        }
      }

      if ((options->n && !options->b) || (options->b && ch != '\n')) {
        fprintf(stdout, "%6d\t", line_number++);
      }
    }

    show_non_printing(ch, options);
    
    prev = ch;
  }
}