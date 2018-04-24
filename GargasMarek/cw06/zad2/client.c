#define GNU_SOURCE
#include <stdio.h>
#include <mqueue.h>
#include "header.h"
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include "header.h"
#define failure_exit(message) { fprintf(stderr, message); exit(EXIT_FAILURE);}

mqd_t server_queue;
mqd_t client_queue;
int client_id;
char client_path[20];

void register_request(msg message);
void mirror_request(msg message);
void calc_request(msg message);
void time_request(msg message);
void end_request(msg message);
void stop_request(msg message);
void int_handler(int signo);

int main()
{
    struct mq_attr attr;
    msg message;

    server_queue = mq_open(server_path, O_WRONLY, 0644);
    if(server_queue == -1) printf("error");

    attr.mq_flags = 0;
    attr.mq_curmsgs = 0;
    attr.mq_maxmsg = MAX_MQSIZE;
    attr.mq_msgsize = MSG_SIZE;
    sprintf(client_path, "/%d", getpid());
    client_queue = mq_open(client_path, O_RDONLY | O_CREAT | O_EXCL, 0666, &attr);
    if(client_queue == -1) printf("error");


    if(signal(SIGINT, int_handler) == SIG_ERR) failure_exit("Registering INT failed!");

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
    if(mq_close(client_queue) == -1) failure_exit("Closing client queue failed");
    mq_unlink(client_path);
    exit(EXIT_SUCCESS);
}

void register_request(msg message){
    sprintf(message.text, "%d", client_queue);
    message.type = REGISTER;
    message.sender_pid = getpid();
    if(mq_send(server_queue, (char*)&message, MSG_SIZE, 1) == -1) failure_exit("REGISTER request failed");
    if(mq_receive(client_queue, (char*)&message, MSG_SIZE, 0) == -1) failure_exit("catching REGISTER response failed!");
    if(sscanf(message.text, "%d", &client_id) < 1) failure_exit("scanning REGISTER response failed!");
    if(client_id < 0) failure_exit("Server cannot have more clients!");

    printf("Client registered! My session nr is %d!\n", client_id);
}


void mirror_request(msg message){
    message.type = MIRROR;
    message.sender_pid = getpid();
    if(fgets(message.text, MSG_SIZE, stdin) == NULL){
        printf("Too many characters!\n");
        return;
    }
    if(mq_send(server_queue, (char*)&message, MSG_SIZE, 1) == -1) failure_exit("MIRROR request failed!");
    if(mq_receive(client_queue, (char*)&message, MSG_SIZE, 0) == -1) failure_exit("catching MIRROR response failed!");
    printf("%s", message.text);
}


void calc_request(msg message){
    message.type = CALC;
    message.sender_pid = getpid();
    if(fgets(message.text, MAX_LENGTH, stdin) == NULL){
        printf("Too many characters!\n");
        return;
    }
    if(mq_send(server_queue, (char*)&message, MSG_SIZE, 1) == -1) failure_exit("CALC request failed!");
    if(mq_receive(client_queue, (char*)&message, MSG_SIZE, 0) == -1) failure_exit("catching CALC response failed!");
    printf("%s", message.text);
}


void time_request(msg message){
    message.type = TIME;
    message.sender_pid = getpid();
    if(mq_send(server_queue, (char*)&message, MSG_SIZE, 1) == -1) failure_exit("TIME request failed!");
    if(mq_receive(client_queue, (char*)&message, MSG_SIZE, 0) == -1) failure_exit("catching TIME response failed!");
    printf("%s", message.text);
}


void end_request(msg message){
    message.type = END;
    message.sender_pid = getpid();
    if(mq_send(server_queue, (char*)&message, MSG_SIZE, 1) == -1) failure_exit("END request failed!");
}


void stop_request(msg message){
    message.type = STOP;
    message.sender_pid = getpid();
    if(mq_send(server_queue, (char*)&message, MSG_SIZE, 1) == -1) failure_exit("STOP request failed!");
}


void int_handler(int signo){
    exit(2);
}
