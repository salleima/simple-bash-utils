#include "grep.h"

int main(int argc, char **argv) {
  options options = {0};

  data grep_data = {0};

  if (argc > 2) {
    grep_data = command_line_processing(argc, argv, &options);
  }

  if (argc <= 2 || options.err) {
    if (!options.s) {
      fprintf(stderr, "usage: grep [options] template [file_name]\n");
      return EXIT_FAILURE;
    }
  }

  if (argc - optind > 1) grep_data.multiple_files = 1;

  for (int i = optind; i < argc; i++) {
    int fp = open(argv[i], O_RDONLY);
    if (fp == -1) {
      if (!options.s)
        fprintf(stderr, "grep: %s: No such file or directory\n", argv[i]);
      continue;
    }

    char *src = NULL;
    struct stat buff = {0};
    stat(argv[i], &buff);

    if ((src = (char *)mmap(0, buff.st_size, PROT_READ, MAP_SHARED, fp, 0)) ==
        MAP_FAILED) {
      if (options.c) {
        if (grep_data.multiple_files > 1) fprintf(stdout, "%s:", argv[i]);
        fprintf(stdout, "0\n");
      }
      continue;
    }
    grep(src, buff.st_size, &grep_data, argv[i], &options);
    munmap(src, buff.st_size);
  }
  if (grep_data.template) free(grep_data.template);
  return EXIT_SUCCESS;
}
