#define _BSD_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LINE_MAX 256

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("Wrong arguments.\nusage:\nmaster path\n");
        exit(1);
    }
    if(mkfifo(argv[1], S_IWUSR | S_IRUSR)){
        printf("Error while creating fifo.\n");
        exit(1);
    }
    FILE* pipe = fopen(argv[1], "r");
    if(pipe == NULL){
        printf("Error while opening file.\n");
        exit(1);
    }
    char buffer[LINE_MAX+1];

    while (fgets(buffer, LINE_MAX+1, pipe) != NULL) {
        write(1, buffer, strlen(buffer));
    }
    fclose(pipe);
    return 0;
}