#include "cat.h"

int main(int argc, char **argv) {
  options options = {0};

  get_options(argc, argv, &options);

  for (int i = optind; i < argc; i++) {
    int fp = open(argv[i], O_RDONLY);

    if (fp == -1) {
      fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
      return EXIT_FAILURE;
    }

    char *src = NULL;
    struct stat buff = {0};
    stat(argv[i], &buff);

    if ((src = (char *)mmap(0, buff.st_size, PROT_READ, MAP_SHARED, fp, 0)) ==
        MAP_FAILED) {
      continue;
    }

    cat(src, buff.st_size, &options);
    munmap(src, buff.st_size);
  }

  return EXIT_SUCCESS;
}
