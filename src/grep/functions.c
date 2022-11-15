#include "grep.h"

void grep(char *src, int file_size, data *grep_data, char *filename,
          options *options) {
  int cur_len = BUF_SIZE;
  char *str = (char *)calloc(cur_len, sizeof(char));
  int cur_size = 0;

  int cflags = 0;
  if (options->i) cflags |= REG_ICASE;
  if (options->e || options->f) cflags |= REG_EXTENDED;

  regex_t regex;
  if (regcomp(&regex, grep_data->template, cflags) != 0) {
    regfree(&regex);
    if (str) free(str);
    return;
  }

  int line_number = 0;
  int matches_count = 0;
  int ind = 0;
  while (cur_size != file_size) {
    line_number++;
    while (*src != '\n' && cur_size != file_size) {
      str[ind++] = *src;
      if (ind + 2 >= cur_len) {
        str = reallocation(str, &cur_len);
        if (str == NULL) exit(1);
      }
      src++;
      cur_size++;
    }
    str[ind] = '\0';

    regmatch_t match;

    if (regexec(&regex, str, 1, &match, 0) == options->v) {
      if (cur_size == file_size && strlen(str) == 0) {
        break;
      } else {
        matches_count++;

        if (grep_data->multiple_files && !options->h && !options->c &&
            !options->l) {
          fprintf(stdout, "%s:", filename);
        }

        if (!options->c && !options->l) {
          if (options->n) {
            fprintf(stdout, "%d:", line_number);
          }
          if (options->o && !options->v) {
            char *tmp = str;
            while (regexec(&regex, tmp, 1, &match, 0) != REG_NOMATCH) {
              for (int i = (int)match.rm_so; i < (int)match.rm_eo; i++)
                fprintf(stdout, "%c", tmp[i]);

              tmp += (int)match.rm_eo;
              fprintf(stdout, "\n");
            }

          } else {
            fprintf(stdout, "%s\n", str);
          }
        }
      }
    }

    ind = 0;
    src++;
  }

  if (options->c && !options->l) {
    if (grep_data->multiple_files && !options->h)
      fprintf(stdout, "%s:", filename);
    fprintf(stdout, "%d\n", matches_count);
  } else if (options->c && options->l) {
    if (grep_data->multiple_files && !options->h)
      fprintf(stdout, "%s:", filename);
    fprintf(stdout, matches_count > 0 ? "1\n" : "0\n");
  }

  if (options->l && matches_count > 0) fprintf(stdout, "%s\n", filename);

  regfree(&regex);
  if (str) free(str);
}

data command_line_processing(int argc, char **argv, options *options) {
  data grep_data = {0};
  grep_data.template = (char *)calloc(BUF_SIZE, sizeof(char));
  grep_data.max_template_size = BUF_SIZE;

  int opt;

  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1) {
    switch (opt) {
      case 'e':
        options->e = 1;
        template_processing(optarg, &grep_data);
        break;
      case 'i':
        options->i = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'f':
        options->f = 1;
        template_processing_from_file(optarg, options, &grep_data);
        break;
      case 'o':
        options->o = 1;
        break;
      default:
        options->err = 1;
        break;
    }
  }

  if (!options->e && !options->f) {
    template_processing(argv[optind], &grep_data);
    optind++;
  }

  return grep_data;
}

void template_processing(char *tmp, data *grep_data) {
  int tmp_size = strlen(tmp);
  if (grep_data->cur_template_size == 0) {
    while (tmp_size > grep_data->max_template_size)
      reallocation(grep_data->template, &grep_data->max_template_size);

    int i = 0;
    for (; i < tmp_size; i++) grep_data->template[i] = tmp[i];

    grep_data->template[i] = '\0';
    grep_data->cur_template_size = i;

  } else {
    while (tmp_size + grep_data->cur_template_size >
           grep_data->max_template_size)
      reallocation(grep_data->template, &grep_data->max_template_size);

    int i = grep_data->cur_template_size;
    grep_data->template[i++] = '|';
    for (; i <= tmp_size + grep_data->cur_template_size; i++)
      grep_data->template[i] = tmp[i - grep_data->cur_template_size - 1];
    grep_data->template[i] = '\0';
    grep_data->cur_template_size = i;
  }
}

void template_processing_from_file(char *filename, options *options,
                                   data *grep_data) {
  FILE *fp = NULL;
  size_t len = 1024;
  char *str = (char *)calloc(len, sizeof(char));

  if ((fp = fopen(filename, "rb")) == NULL) {
    if (!options->s)
      fprintf(stderr, "grep: %s: No such file or directory\n", filename);
  } else {
    while (getline(&str, &len, fp) != -1) {
      len = strlen(str);
      char *new_str = (char *)calloc(len, sizeof(char));
      strncpy(new_str, str, len - 1);
      template_processing(new_str, grep_data);
      if (new_str) free(new_str);
    }
    fclose(fp);
  }
  if (str) free(str);
}

char *reallocation(char *tmp, int *max_tmp_size) {
  char *cpy = tmp;
  (*max_tmp_size) *= 2;
  if ((tmp = realloc(tmp, (*max_tmp_size))) == NULL) free(cpy);
  return tmp;
}