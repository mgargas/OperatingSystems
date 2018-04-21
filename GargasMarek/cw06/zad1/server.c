#define _XOPEN_SOURCE
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "header.h"

int client_counter = -1;
int client_data[MAX_CLIENTS][2]; // clientPID, clientQID
int received_end;

int get_client_qid(pid_t client_pid);
void register_service(msg message);
void mirror_service(msg message);
void calc_service(msg message);
void time_service(msg message);
void end_service();
void stop_service(msg message);
void init_client_data();
int add_to_client_data(pid_t client_pid, key_t client_qid);
int delete_from_client_data(pid_t client_pid, key_t client_qid);
void int_handler(int signo);


int main() {
    key_t key;
    int server_queue;

    char* path = getenv("HOME");
    key = ftok(path, SERVER_KEY);

    if(signal(SIGINT, int_handler) == SIG_ERR) printf("Registering INT failed!");

    server_queue = msgget(key, 0666 | IPC_CREAT);
    msg message;
    struct msqid_ds currentState;
    received_end = 0;
    init_client_data();
    while(1){
        if(received_end) {
            if(msgctl(server_queue, IPC_STAT, &currentState) == -1) printf("Getting current state of public queue failed!\n");
            if(currentState.msg_qnum == 0) break;
        }
        if(msgrcv(server_queue, &message, MSG_SIZE, 0, 0) == -1) printf("Receiving message by the server failed!");
        switch (message.type){
            case REGISTER:
                register_service(message);
                break;
            case MIRROR:
                mirror_service(message);
                break;
            case CALC:
                calc_service(message);
                break;
            case TIME:
                time_service(message);
                break;
            case END:
                end_service();
                break;
            case STOP:
                stop_service(message);
                break;
        }
    }
    msgctl(server_queue, IPC_RMID, NULL);
    return 0;
}
void register_service(msg message)
{
    client_counter++;
    pid_t client_PID = message.sender_pid;
    key_t client_queue;
    sscanf(message.text, "%d", &client_queue);
    if(client_counter < MAX_CLIENTS){
        add_to_client_data(client_PID, client_queue);
        sprintf(message.text,"%d", client_counter);
    }else{
        sprintf(message.text, "%d", client_counter);
    }
    message.sender_pid = getpid();
    message.type = REGISTER;
    if(msgsnd(client_queue, &message, MSG_SIZE, 0) == -1) {
        printf("REGISTER response failed!");
    }
}

void mirror_service(msg message){
    int client_queue = get_client_qid(message.sender_pid);
    message.sender_pid = getpid();
    int msg_len = (int) strlen(message.text);
    if(message.text[msg_len-1] == '\n') msg_len--;

    for(int i=0; i < msg_len / 2; i++) {
        char buff = message.text[i];
        message.text[i] = message.text[msg_len - i - 1];
        message.text[msg_len - i - 1] = buff;
    }
    if(msgsnd(client_queue, &message, MSG_SIZE, 0) == -1) {
       printf("MIRROR response failed!");
    }
}


void calc_service(msg message){
    int client_queue = get_client_qid(message.sender_pid);
    message.sender_pid = getpid();
    char cmd[1023 + 12];
    sprintf(cmd, "echo '%s' | bc", message.text);
    FILE* calc = popen(cmd, "r");
    fgets(message.text, MAX_LENGTH, calc);
    pclose(calc);
    if(msgsnd(client_queue, &message, MSG_SIZE, 0) == -1) printf("CALC response failed!");
}


void time_service(msg message){
    int client_queue = get_client_qid(message.sender_pid);
    message.sender_pid = getpid();
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(message.text, "now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    if(msgsnd(client_queue, &message, MSG_SIZE, 0) == -1) printf("TIME response failed!");
}


void stop_service(msg message){
    int client_queue = get_client_qid(message.sender_pid);
    if(delete_from_client_data(message.sender_pid, client_queue) == 0) client_counter--;
    else printf("nie dziala");
}


void end_service(){
    received_end = 1;
}


int get_client_qid(pid_t client_pid){
    for(int i=0; i<MAX_CLIENTS; i++){
        if(client_data[i][0] == client_pid) return client_data[i][1];
    }
    printf("Couldnd't find client with this client_pid");
    return -1;
}


void init_client_data(){
    for(int i=0;i<MAX_CLIENTS;i++) {
        client_data[i][0] = -1;
        client_data[i][1] = -1;
    };
}


int add_to_client_data(pid_t client_pid, key_t client_qid){
    for(int i=0;i<MAX_CLIENTS;i++) {
        if(client_data[i][0] == -1 && client_data[i][1] == -1){
                client_data[i][0] = client_pid;
                client_data[i][1] = client_qid;
                return 1;
        }
    };
    return -1;
}


int delete_from_client_data(pid_t client_pid, key_t client_qid){
    for(int i=0;i<MAX_CLIENTS;i++) {
        if(client_data[i][0] == client_pid && client_data[i][1] == client_qid){
            client_data[i][0] = -1;
            client_data[i][1] = -1;
            return 0;
        }
    };
    return 1;
}


void int_handler(int signo){
    exit(2);
}