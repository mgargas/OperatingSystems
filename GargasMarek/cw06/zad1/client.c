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
#include "header.h"

void mirror_request(msg message);
void register_request(msg message);
int server_queue;
int client_queue;
int client_id;

int main()
{
    key_t key;
    char* path = getenv("HOME");

    key = ftok(path, SERVER_KEY);
    server_queue = msgget(key, 0666 | IPC_CREAT);

    key = ftok(path, getpid());
    client_queue = msgget(key, 0666 | IPC_CREAT | IPC_EXCL);

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
        }
    }

    return 0;
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
    printf("Enter string of characters to Mirror: ");
    if(fgets(message.text, MSG_SIZE, stdin) == NULL){
        printf("Too many characters!\n");
        return;
    }
    if(msgsnd(server_queue, &message, MSG_SIZE, 0) == -1) printf("MIRROR request failed!");
    if(msgrcv(client_queue, &message, MSG_SIZE, 0, 0) == -1) printf("catching MIRROR response failed!");
    printf("%s", message.text);
}
