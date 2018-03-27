GCC = gcc -Wall -std=c99 -ggdb
M_PATH = main



all: build clean tests

build:
	$(GCC) -o $(M_PATH) main.c
	$(GCC) -o memory_killer memory_killer.c
	$(GCC) -o time_killer time_killer.c

clean:
	rm -f *.o


tests:
