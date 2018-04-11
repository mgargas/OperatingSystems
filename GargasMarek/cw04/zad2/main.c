#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <bits/siginfo.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

int N;
int K;
pid_t* children;
pid_t* requests;
volatile pid_t parent;

#define SIGDIFF (SIGRTMAX - SIGRTMIN)

volatile sig_atomic_t requests_received = 0;
volatile sig_atomic_t alive_children = 0;
volatile sig_atomic_t exit_flag = 0;


int is_number(char* arg) {
    for(int i=0; i<strlen(arg); i++)
        if(arg[i]<'0' || arg[i]>'9') return 0;
    return 1;
}

void request_handler(int _signum, siginfo_t *siginfo, void *_context){
    printf("Parent - %i got SIGUSR1 request from %i\n",getpid(),siginfo->si_pid);
    requests_received++;
    if(requests_received < K) requests[requests_received-1] = siginfo->si_pid;
    if(requests_received==K) {
        kill(siginfo->si_pid, SIGUSR2);
        for(int i=0;i<K-1;i++) kill(requests[i], SIGUSR2);
    } else{
        kill(siginfo->si_pid, SIGUSR2);
    }
}


void confirmation_handler(int _signum, siginfo_t *siginfo, void *_context){
    printf("Child - %i got SIGUSR2 confirmation from %i\n",getpid(),siginfo->si_pid);
    int sig_diff = rand() % SIGDIFF;
    kill(getppid(), SIGRTMIN + sig_diff);
}

void child_result_handler(int _signum, siginfo_t *siginfo, void *_context){
    printf("Child %i stopped.\n", siginfo->si_pid);
    alive_children--;

    int i; for (i = 0; i < N; ++i) {
        if (children[i] == siginfo->si_pid) children[i] = 0;
    }

    int exit_status;
    wait(&exit_status);
    exit_status = WIFEXITED(&exit_status);
    printf(
            "Child pid: %i, exited with status: %i \n",
            siginfo->si_pid,
            exit_status
    );
}

void rt_handler(int signum, siginfo_t *siginfo, void *_context) {
    printf(
            "Got signal SIGMIN+%i, from child pid: %i\n",
            signum - SIGRTMIN,
            siginfo->si_pid
    );
}

void int_handler(int _signum, siginfo_t *_siginfo, void *_context) {
    if (getpid() != parent) return;
    /**/ printf("Received SIGINT.\n");
    int i; for (i = 0; i < alive_children; ++i) kill(children[i], SIGKILL);
    exit_flag = 1;
}

void kill_children() {
    /**/ printf("Terminating children.\n");
    int i; for (i = 0; i < N; ++i) if (children[i]) kill(children[i], SIGKILL);
}

int main(int argc, char** argv) {

    if(argc!=3 || !is_number(argv[1]) || !is_number(argv[2])) {
        printf("Wrong arguments.\nUsage:\n.\\main N K\n");
        exit(1);
    }
    N = atoi(argv[1]);
    K = atoi(argv[2]);
	if(K>N) {
		printf("K>N: that program would never finish.\n");
		exit(1);
	}
    children = malloc(sizeof(pid_t)*N);
    requests = malloc(sizeof(pid_t)*N);

    sigset_t block_signals;
    sigset_t used_signals;
    sigemptyset(&used_signals);
    sigaddset(&used_signals, SIGUSR1);
    sigaddset(&used_signals, SIGCHLD);
    sigaddset(&used_signals, SIGINT);
    for (int i = SIGRTMIN; i <= SIGRTMAX; ++i) sigaddset(&used_signals, i);

    struct sigaction sig_act;
    sig_act.sa_mask = block_signals;
    sig_act.sa_flags = SA_SIGINFO;

    sig_act.sa_sigaction = &rt_handler;
    for (int i = 0; i < SIGDIFF; ++i) sigaction(SIGRTMIN+i, &sig_act, 0);

    sig_act.sa_sigaction = &request_handler;
    sigaction(SIGUSR1, &sig_act, NULL);

    sig_act.sa_sigaction = &child_result_handler;
    sigaction(SIGCHLD, &sig_act, NULL);
    
    sig_act.sa_sigaction = &int_handler;
    sigaction(SIGINT, &sig_act, NULL);



    pid_t pid;
    parent = getpid();
    int j=0;
    while(j<N) {
        pid = fork();
        alive_children++;
        if (pid) children[j] = pid;
        else {
            srand((unsigned int)(getpid()*13)%10);
            unsigned int sleep_time = (unsigned int) (rand() % 10);
            sleep(sleep_time);
            printf("%i: awoke from sleep , sending kill to parent.\n", getpid());
            kill(getppid(), SIGUSR1);
            sig_act.sa_sigaction = &confirmation_handler;
            sigaction(SIGUSR2, &sig_act, NULL);
            pause();
            printf("%i: Received response and sent realtime signal, dying in piece now.\n", getpid());
            return sleep_time;
        }
        j++;
    }
    while (alive_children) if (exit_flag) kill_children();
    return 0;
}
