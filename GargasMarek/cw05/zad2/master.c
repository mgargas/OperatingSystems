#define _BSD_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LINE_MAX 256

int main(int argc, char **argv) {
    mkfifo("", S_IWUSR | S_IRUSR);
    FILE* pipe = fopen("./kk", "r");
    char buffer[LINE_MAX];

    while (fgets(buffer, LINE_MAX, pipe) != NULL) {
        write(1, buffer, strlen(buffer));
    }
    fclose(pipe);
    return 0;
}