GCC = gcc -Wall -std=c99 -ggdb
M_PATH = server
M_PATH2 = client

all: build clean tests

build:
	$(GCC) -o $(M_PATH) server.c
	$(GCC) -o $(M_PATH2) client.c

clean:
	rm -f *.o

tests:

run_server:
	./server

run_client:
	./client
