GCC = gcc -Wall -ggdb
M_PATH = main
M_PATH2 = main_2

all: build clean tests

build:
	$(GCC) -o $(M_PATH) main.c
	$(GCC) -o $(M_PATH2) main_2.c

clean:
	rm -f *.o

tests:
