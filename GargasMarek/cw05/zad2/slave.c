#define _BSD_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <memory.h>

#define LINE_MAX 256

int is_number(char* arg) {
    for(int i = 0; i < strlen(arg); i++)
        if(arg[i]<'0' || arg[i]>'9') return 0;
    return 1;
}

int main(int argc, char** argv) {
    if(argc != 3) {
        printf("Wrong arguments.\nusage:\nslave path N\n");
        exit(1);
    }
    if(!is_number(argv[2])) {
        printf("Wrong arguments.\nN has to be a natural number.\n");
        exit(1);
    }
    int N = atoi(argv[2]);
    int pipe = open(argv[1], O_WRONLY);
    if(pipe == -1) {
        printf("Error while opening file.\n");
        exit(1);
    }
    char sys_date[LINE_MAX];
    char message[LINE_MAX];
    FILE* date;
    for(int i=0;i<N;i++){
        date = popen("date", "r");
        if(date == NULL){
            printf("Error while opening file with date.\n");
            exit(1);
        }
        fgets(sys_date, LINE_MAX, date);
        sprintf(message, "Slave: %d - %s", getpid(), sys_date);
        write(pipe, message, strlen(message));
        fclose(date);
        sleep(4);
    }
    close(pipe);
    return 0;
}