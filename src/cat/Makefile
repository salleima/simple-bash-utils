CC = gcc
FLAGS = -Wall -Wextra -Werror
FILES = main.c functions.c

all: cat

cat: 
	$(CC) $(FLAGS) $(FILES) -o cat

rebuild: clean all

clean:
	-rm -rf cat