CC = gcc
FLAGS = -Wall -Wextra -Werror
FILES = main.c functions.c

all: grep

grep: 
	$(CC) $(FLAGS) $(FILES) -o grep

rebuild: clean all

clean:
	-rm -rf grep