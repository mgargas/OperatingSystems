#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

int is_working = 0;
int is_child_created = 0;
pid_t pid;

int is_paused = 0; //0 - false

void tstp_handler(int signum){
    if(is_paused){
        is_paused = 0;
    }
    else if(!is_paused){
        printf("\rOczekuję na CTRL+Z - kontynuacja albo CTR+C - zakonczenie programu\n");
        is_paused = 1;
        if(pid != 0) kill(pid,SIGKILL);
        sigset_t mask;
        sigfillset(&mask);
        sigdelset(&mask, SIGTSTP);
        sigdelset(&mask, SIGINT);
        sigsuspend(&mask);
    }
}

void int_handler(int signum){
    printf("\rOdebrano sygnał SIGINIT\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[]) {
    signal(SIGTSTP, tstp_handler);

    struct sigaction act;
    act.sa_handler = int_handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGINT, &act, NULL);

    char* args[] = {"sh","./date.sh",NULL};


    while(1) {
        int status;
        if(!is_paused){
            pid = fork();
            if(pid < 0){
                perror("fork() error");
            }
            if(pid == 0) {
                status = execvp(args[0], args);
                exit(EXIT_SUCCESS);
            }
        }
        wait(&status);
    }
    return 0;
}


