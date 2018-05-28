CC=gcc
CFLAGS=-Wall -std=gnu99 -lpthread -lm

.PHONY: clean

all: main

main: main.c
	$(CC) $< -o $@ $(CFLAGS)

test: main
	mkdir -p out
	echo "threads,filter size,time (ms)" > Times.txt
	bash -c 'for FILTER in {03,10,65}; do                                       \
				for THREAD in {1,2,4,8}; do                         \
					./main $$THREAD                             \
					image_in.pgm                                \
					filters/filter$$FILTER                      \
					out/out.$$THREAD.$$FILTER.pgm; \
				done;                                               \
			done'

clean:
	rm -rf out main
