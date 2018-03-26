#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>



#define LINE_MAX 256


char** parse_arguments(char* line){
    int size = 0;
    char** args = NULL;
    char delimiters[3] = {' ','\n','\t'};
    char* a = strtok(line, delimiters);
    while(a != NULL){
        size++;
        args = realloc(args, sizeof(char*) * size);
        if(args == NULL){
            exit(EXIT_FAILURE);
        }
        args[size-1] = a;
        a = strtok(NULL, delimiters);
    }
    args = realloc(args, sizeof(char*) * (size+1));
    if(args == NULL){
        exit(EXIT_FAILURE);
    }
    args[size] = NULL;

    return args;
}

int main(int argc, char const *argv[]) {
    if(argc < 2){
        printf("Pass only [filename]");
        exit(EXIT_FAILURE);
    }
    FILE* file = fopen(argv[1], "r");
    if(!file){
        printf("Enable to open this file: %s", argv[1]);
        exit(EXIT_FAILURE);
    }
    char line[LINE_MAX];
    while(fgets(line, LINE_MAX, file ) != NULL){
        char** args = parse_arguments(line);
        pid_t pid;
        pid = vfork();
        if(!pid){
            if(execvp(args[0], args) < 0){
                exit(EXIT_FAILURE);
            };

        }
        int status;
        wait(&status);
        if(status){
            fprintf(stderr, "Error while running command: %s\n", args[0]);
            exit(EXIT_FAILURE);
        }

    }
    fclose(file);
    return 0;
}