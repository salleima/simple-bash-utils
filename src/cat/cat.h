#ifndef CAT_H
#define CAT_H

#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int E;
  int T;
} options;

void cat(char *src, int file_size, options *options);
void get_options(int argc, char **agrv, options *options);
void numbering(char *str, int *line, int bufsize);
void show_non_printing(int ch, options *options);

#endif // CAT_H