#define _BSD_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define LINE_MAX 256

int main(int argc, char** argv) {
    int pipe = open("./kk", O_WRONLY);
    write(pipe, "komunikat", LINE_MAX);
    close(pipe);
    return 0;
}