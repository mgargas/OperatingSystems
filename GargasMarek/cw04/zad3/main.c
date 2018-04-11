#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <bits/siginfo.h>
#include <sys/time.h>
#include <time.h>


pid_t parent;
pid_t child;


int sent_signals = 0;
int received_signals = 0;
int type;
int L;


int is_number(char* arg) {
    for(int i=0; i<strlen(arg); i++) {
        if(arg[i]<'0' || arg[i]>'9') return 0;
    }
    return 1;
}

void USR1_handler(int _signum, siginfo_t *siginfo, void *_context){
    if(getpid() == parent){
        printf("%i got USR1 from %i", getpid(), siginfo->si_pid);
        received_signals++;
        printf("\tReceived from child: %i\n",received_signals);

    }else{
        printf("%i got USR1 from %i", getpid(), siginfo->si_pid);
        received_signals++;
        printf("\tReceived by child: %i\n",received_signals);
        //usleep(50000);
        kill(getppid(),SIGUSR1);

    }
}

void USR2_handler(int _signum, siginfo_t *siginfo, void *_context){
    if(getpid() != parent) {
        printf("%i got USR2 from %i", getpid(), siginfo->si_pid);
        received_signals++;
        printf("\tReceived by child: %i\n",received_signals);
        exit(EXIT_SUCCESS);
    }
}

void INT_handler(int _signum, siginfo_t *siginfo, void *_context){
    if(getpid() == parent) {
        printf("%i got INT from user. I am sending USR2 to %i\n", getpid(), child);
        if(type == 1 || type == 2) kill(child, SIGUSR2);
        else kill(child, SIGRTMAX);
        if(sent_signals < L) {
            int state = 0;
            waitpid(child, &state, 0);
            exit(EXIT_FAILURE);
        }
    }
}

void RTMIN_handler(int _signum, siginfo_t *siginfo, void *_context){
    if(getpid() == parent){
        printf("%i got RTMIN from %i", getpid(), siginfo->si_pid);
        received_signals++;
        printf("\tReceived from child: %i\n",received_signals);

    }else{
        printf("%i got RTMIN from %i", getpid(), siginfo->si_pid);
        received_signals++;
        printf("\tReceived by child: %i\n",received_signals);
        //usleep(50000);
        kill(getppid(),SIGRTMIN);

    }
}

void RTMAX_handler(int _signum, siginfo_t *siginfo, void *_context){
    if(getpid() != parent) {
        printf("%i got RTMAX from %i", getpid(), siginfo->si_pid);
        received_signals++;
        printf("\tReceived by child: %i\n",received_signals);
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char** argv) {
    if(argc!=3) {
        printf("Wrong arguments.\nUsage:\n./main L Type\n");
        exit(EXIT_FAILURE);
    }

    if(!is_number(argv[1]) || !(strcmp(argv[2], "1")==0 || strcmp(argv[2], "2")==0 || strcmp(argv[2], "3")==0)) {
        printf("Wrong arguments.\nUsage:\n./main L Type\nL-natural number\nType=1, 2 or 3\n");
        exit(EXIT_FAILURE);
    }
    L = atoi(argv[1]);
    type = argv[2][0] - '0';

    sigset_t blocked_signals;
    //sigemptyset(&blocked_signals);
    sigfillset(&blocked_signals);
    sigdelset(&blocked_signals,SIGINT);

    struct sigaction sig_act;
    sig_act.sa_mask = blocked_signals;
    sig_act.sa_flags = SA_SIGINFO;
    if(type == 1 || type == 2) {
        sigdelset(&blocked_signals,SIGUSR1);
        sigdelset(&blocked_signals,SIGUSR2);

        sig_act.sa_sigaction = &USR1_handler;
        sigaction(SIGUSR1, &sig_act, NULL);

        sig_act.sa_sigaction = &USR2_handler;
        sigaction(SIGUSR2, &sig_act, NULL);
    }else {
        sigdelset(&blocked_signals,SIGRTMIN);
        sigdelset(&blocked_signals,SIGRTMAX);

        sig_act.sa_sigaction = &RTMIN_handler;
        sigaction(SIGRTMIN, &sig_act, NULL);

        sig_act.sa_sigaction = &RTMAX_handler;
        sigaction(SIGRTMAX, &sig_act, NULL);
    }

    sig_act.sa_sigaction = &INT_handler;
    sigaction(SIGINT, &sig_act, NULL);


    pid_t pid;
    parent = getpid();
    pid = fork();
    if(pid < 0) {
        printf("Could not fork.\n");
        exit(1);
    }
    if(pid) {
        child = pid;
        for (int i = 0; i < L; i++) {
            printf("%i is sending chosen signal to %i\n", getpid(), pid);
            sleep(1);
            if(type == 1 || type == 2)kill(pid, SIGUSR1);
            else kill(pid, SIGRTMIN);
            if(type == 1 || type == 3)usleep(100);
            sent_signals++;
            if(type==2){
                printf("I am waiting for my child's respond\n");
                pause();
            }
        }
        if(type == 1 || type == 2) kill(pid, SIGUSR2);
        else kill(pid, SIGRTMAX);
        sent_signals++;
        int state = 0;
        waitpid(pid, &state, 0);
        printf("Signals sent by parent: %i\n",sent_signals);
    } else{
        while(1) sleep(10);
    }
    return 0;
}