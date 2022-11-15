#ifndef GREP_H
#define GREP_H

#include <fcntl.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUF_SIZE 256

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int err;
} options;

typedef struct {
  char *template;
  int max_template_size;
  int cur_template_size;
  int multiple_files;
} data;

data command_line_processing(int argc, char **argv, options *options);
void template_processing(char *tmp, data *grep_data);
void template_processing_from_file(char *filename, options *options,
                                   data *grep_data);
void grep(char *src, int file_size, data *grep_data, char *filename,
          options *options);
char *reallocation(char *tmp, int *max_tmp_size);

#endif  // GREP_H