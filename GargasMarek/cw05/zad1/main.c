#define _BSD_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <sys/wait.h>
#include <stdlib.h>

#define LINE_MAX 256


char** parse_program_arguments(char *line){
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

char** split_programs(char* line, int* count){
    *count = 0;
    char** args = NULL;
    char delimiters[1] = {'|'};
    char* a = strtok(line, delimiters);
    while(a != NULL){
        (*count)++;
        args = realloc(args, sizeof(char*) * (*count));
        if(args == NULL){
            exit(EXIT_FAILURE);
        }
        args[(*count)-1] = a;
        //printf("%s.\n",args[size-1]);
        a = strtok(NULL, delimiters);
    }
    //printf("%d\n", size);

    return args;
}


int main(int argc, char** argv) {
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
    int count;
    int* fd;
    pid_t pid;
    while(fgets(line, LINE_MAX, file)){
        char** programs = split_programs(line, &count);
        if(count < 5){
	        printf("You have to pass at least 5 commands in line.\n");
                exit(1);
	}
        fd = malloc(sizeof(int) * 2 * (count));
        for(int i = 0; i < count ; i++) {
            if(pipe(&fd[2 * i]) == -1) {
                printf("Error on pipe().\n");
                exit(1);
            }
        }
        for(int i=0; i< count; i++){
            pid = fork();
            if(pid < 0) {
                printf("Error on fork().\n");
                exit(1);
            }
            else if(pid == 0){
                if(i < count - 1) dup2(fd[2 * i + 1], 1);
                if(i > 0) dup2(fd[2 * i - 2], 0);
                close(fd[2 * i + 1]);
                char** args = parse_program_arguments(programs[i]);
                if(execvp(args[0], args) == -1) {
                    printf("Error occured on execution: ");
                    for(int j = 0; args[j] != NULL; j++) printf("%s ", args[j]);
                    printf("\n");
                }
                exit(1);
            }
            close(fd[2 * i + 1]);
            wait(NULL);
        }
    }

    return 0;
}
