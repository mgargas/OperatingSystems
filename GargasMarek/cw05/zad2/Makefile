GCC = gcc -Wall -std=c99 -ggdb
M_PATH = master
M_PATH2 = slave

all: build clean tests

build:
	$(GCC) -o $(M_PATH) master.c
	$(GCC) -o $(M_PATH2) slave.c

clean:
	rm -f *.o

tests:

run_master:
	./master fifo

run_slave:
	./slave fifo 5
