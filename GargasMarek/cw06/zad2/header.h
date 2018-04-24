#ifndef HEADER_H
#define HEADER_H

#define SERVER_KEY 101
#define MAX_LENGTH 1023
#define MAX_CLIENTS 10
#define MAX_MQSIZE 9
#define MSG_SIZE  sizeof(msg)

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


const char server_path[] = "/server";

#endif //HEADER_H
