CC = gcc
FLAGS = -g -std=c99 -w -pipe -O3 -Wextra -Werror -pedantic

all: run

run: equivalence.o main.o
	$(CC) $(FLAGS) $^ -o $@ -lm

clean:
	rm -f *.o run
