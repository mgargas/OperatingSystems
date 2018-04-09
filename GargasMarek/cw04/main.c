#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

int is_paused = 0; //0 - false

void tstp_handler(int signum){
    if(is_paused){
        is_paused = 0;
    }
    else if(!is_paused){
        printf("\rOczekuję na CTRL+Z - kontynuacja albo CTR+C - zakonczenie programu\n");
        is_paused = 1;
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
    time_t act_time;

    while(!is_paused) {
        char buffer[30];
        act_time = time(NULL);
        strftime(buffer, sizeof(buffer), "%H:%M:%S", localtime(&act_time));
        printf("%s\n", buffer);
        sleep(1);
    }
    return 0;
}