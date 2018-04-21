#define _XOPEN_SOURCE
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "header.h"

int client_counter = -1;
int client_data[MAX_CLIENTS][2]; // clientPID, clientQID

int prepare_message(msg message, long type, pid_t sender_pid, char* text);
int get_client_qid(pid_t client_pid);
void register_service(msg message);
void mirror_service(msg message);

int main() {
    key_t key;
    int server_queue;

    char* path = getenv("HOME");
    key = ftok(path, SERVER_KEY);

    server_queue = msgget(key, 0666 | IPC_CREAT);
    msg message;
    while(1){
        msgrcv(server_queue, &message, MSG_SIZE, 0, 0);
        switch (message.type){
            case REGISTER:
                register_service(message);
                break;
            case MIRROR:
                mirror_service(message);
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
        client_data[client_counter][0] = client_PID;
        client_data[client_counter][1] = client_queue;
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

int get_client_qid(pid_t client_pid){
    for(int i=0; i<MAX_CLIENTS; i++){
        if(client_data[i][0] == client_pid) return client_data[i][1];
    }
    return -1;
}