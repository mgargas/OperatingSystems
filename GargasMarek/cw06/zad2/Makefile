GCC = gcc -Wall -std=c99 -ggdb
M_PATH = server
M_PATH2 = client

all: build clean tests

build:
	$(GCC) -o $(M_PATH) server.c -lrt
	$(GCC) -o $(M_PATH2) client.c -lrt

clean:
	rm -f *.o

tests:

run_server:
	./server

run_client:
	./client
