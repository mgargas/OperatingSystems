//
// Created by marek on 19.04.18.
//
#define _XOPEN_SOURCE
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <memory.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "header.h"


int server_queue;
int client_queue;
int client_id;


void register_request(msg message);
void mirror_request(msg message);
void calc_request(msg message);
void time_request(msg message);
void end_request(msg message);
void stop_request(msg message);
void int_handler(int signo);

int main()
{
    key_t key;
    char* path = getenv("HOME");

    key = ftok(path, SERVER_KEY);
    server_queue = msgget(key, 0666 | IPC_CREAT);

    key = ftok(path, getpid());
    client_queue = msgget(key, 0666 | IPC_CREAT | IPC_EXCL);

    if(signal(SIGINT, int_handler) == SIG_ERR) printf("Registering INT failed!");

    msg message;
    register_request(message);
    char cmd[20];
    while(1) {
        printf("Enter your request: ");
        if(fgets(cmd, 20, stdin) == NULL){
            printf("Error reading your command!\n");
            continue;
        }
        message.sender_pid = getpid();
        int n = strlen(cmd);
        if(cmd[n-1] == '\n') cmd[n-1] = 0;
        if(strcmp(cmd, "mirror") == 0) {
            mirror_request(message);
        } else if(strcmp(cmd, "calc") == 0){
            calc_request(message);
        } else if(strcmp(cmd, "time") == 0){
            time_request(message);
        } else if(strcmp(cmd, "end") == 0){
            end_request(message);
            break;
        } else if(strcmp(cmd, "stop") == 0){
            stop_request(message);
            break;

        }else printf("Wrong command!\n");
    }
    msgctl(client_queue, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}

void register_request(msg message){
    sprintf(message.text, "%d", client_queue);
    message.type = REGISTER;
    message.sender_pid = getpid();
    if(msgsnd(server_queue, &message, MSG_SIZE, 0) == -1) printf("REGISTER request failed");
    if(msgrcv(client_queue, &message, MSG_SIZE, 0, 0) == -1) printf("catching REGISTER response failed!");
    if(sscanf(message.text, "%d", &client_id) < 1) printf("scanning REGISTER response failed!");
    if(client_id < 0) printf("Server cannot have more clients!");

    printf("Client registered! My session nr is %d!\n", client_id);
}


void mirror_request(msg message){
    message.type = MIRROR;
    message.sender_pid = getpid();
    if(fgets(message.text, MSG_SIZE, stdin) == NULL){
        printf("Too many characters!\n");
        return;
    }
    if(msgsnd(server_queue, &message, MSG_SIZE, 0) == -1) printf("MIRROR request failed!");
    if(msgrcv(client_queue, &message, MSG_SIZE, 0, 0) == -1) printf("catching MIRROR response failed!");
    printf("%s", message.text);
}


void calc_request(msg message){
    message.type = CALC;
    message.sender_pid = getpid();
    if(fgets(message.text, MAX_LENGTH, stdin) == NULL){
        printf("Too many characters!\n");
        return;
    }
    if(msgsnd(server_queue, &message, MSG_SIZE, 0) == -1) printf("CALC request failed!");
    if(msgrcv(client_queue, &message, MSG_SIZE, 0, 0) == -1) printf("catching CALC response failed!");
    printf("%s", message.text);
}


void time_request(msg message){
    message.type = TIME;
    message.sender_pid = getpid();
    if(msgsnd(server_queue, &message, MSG_SIZE, 0) == -1) printf("TIME request failed!");
    if(msgrcv(client_queue, &message, MSG_SIZE, 0, 0) == -1) printf("catching TIME response failed!");
    printf("%s", message.text);
}


void end_request(msg message){
    message.type = END;
    message.sender_pid = getpid();
    if(msgsnd(server_queue, &message, MSG_SIZE, 0) == -1) printf("END request failed!");
}


void stop_request(msg message){
    message.type = STOP;
    message.sender_pid = getpid();
    if(msgsnd(server_queue, &message, MSG_SIZE, 0) == -1) printf("STOP request failed!");
}


void int_handler(int signo){
    exit(2);
}