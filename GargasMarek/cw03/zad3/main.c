#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>



#define LINE_MAX 256


void display_usage(char* name, struct rusage *before, struct rusage *after) {
    time_t user_time =
            (after->ru_utime.tv_sec - before->ru_utime.tv_sec) * 1000000 +
            after->ru_utime.tv_usec - before->ru_utime.tv_usec;
    time_t system_time =
            (after->ru_stime.tv_sec - before->ru_stime.tv_sec) * 1000000 +
            after->ru_stime.tv_usec - before->ru_stime.tv_usec;
    printf("Finished execution of '%s':\n"
                   "User time: %ld us\n"
                   "System time: %ld us\n\n",
           name, user_time, system_time);
}

int set_limits(char *time, char *memory) {
    int time_limit = atoi(time);
    struct rlimit r_limit_cpu;
    r_limit_cpu.rlim_max = (rlim_t) time_limit;
    r_limit_cpu.rlim_cur = (rlim_t) time_limit;
    if (setrlimit(RLIMIT_CPU, &r_limit_cpu) != 0) {
        printf("Enable to set time limit");
        return -1;
    }

    int memory_limit = atoi(memory);
    struct rlimit r_limit_memory;
    r_limit_memory.rlim_max = (rlim_t) memory_limit * 1024 * 1024;
    r_limit_memory.rlim_cur = (rlim_t) memory_limit * 1024 * 1024;

    if (setrlimit(RLIMIT_DATA, &r_limit_memory) != 0) {
        printf("Enable to set memory limit");
        return -1;
    }
    return 0;
}

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

int main(int argc, char *argv[]) {
    if(argc < 4){
        printf("Wrong number of arguments! Pass [filename] [time_limit] [memory_limit]");
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
        struct rusage usage_before, usage_after;
        pid_t pid;
        getrusage(RUSAGE_CHILDREN, &usage_before);
        pid = vfork();
        if(!pid){
            set_limits(argv[2], argv[3]);
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
        getrusage(RUSAGE_CHILDREN, &usage_after);
        display_usage(args[0], &usage_before, &usage_after);
    }
    fclose(file);
    return 0;
}