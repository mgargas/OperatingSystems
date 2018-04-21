//
// Created by marek on 19.04.18.
//

#ifndef HEADER_H
#define HEADER_H

#define SERVER_KEY 101
#define MAX_LENGTH 1023
#define MAX_CLIENTS 2

enum communicate {
    MIRROR = 1,
    CALC = 2,
    TIME = 3,
    END = 4,
    REGISTER = 5,
    STOP = 6
};

typedef struct msg{
    long type;
    pid_t sender_pid;
    char text[MAX_LENGTH];
} msg;

void print_msg(msg message){
    printf("Message: \n %li\n %i\n %s\n",message.type, message.sender_pid, message.text);
}

const size_t MSG_SIZE = sizeof(msg) - sizeof(long);
#endif //HEADER_H
