#include <stdio.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <memory.h>
#include <sys/time.h>
#include "sys.h"
#include "lib.h"

long int getTime(struct timeval *t) {
    return (long int)t->tv_sec * 1000000 + (long int)t->tv_usec;
}

int main(int argc, char *argv[]) {
    if(argc != 6 && argc !=7){
        printf("You have passed wrong number of arguments!\n");
        return -1;
    }
    int record_amount;
    int record_size;
    if(argc == 6){
        record_amount = (int) strtol(argv[3], '\0', 10);
        record_size = (int) strtol(argv[4], '\0', 10);
    }else{
        record_amount = (int) strtol(argv[4], '\0', 10);
        record_size = (int) strtol(argv[5], '\0', 10);
    }


    struct rusage usage;
    long int s_start, s_end, u_start, u_end, r_start, r_end;
    struct timeval real;
    getrusage(RUSAGE_SELF, &usage);
    gettimeofday(&real, 0);
    s_start = getTime(&usage.ru_stime);
    u_start = getTime(&usage.ru_utime);
    r_start = getTime(&real);

    if(strcmp(argv[1],"generate")==0){

        if(strcmp(argv[5], "sys") == 0){
            sys_generate(argv[2], record_amount, record_size);
        }else if(strcmp(argv[5], "lib") == 0){
            lib_generate(argv[2], record_amount, record_size);
        } else{
            printf("You have passed wrong mode as an argument!\n");
            return -1;
        }
    }

    else if(strcmp(argv[1],"sort")==0){
        if(strcmp(argv[5], "sys") == 0){
            sys_sort(argv[2], record_amount, record_size);
        }else if(strcmp(argv[5], "lib") == 0){
            lib_sort(argv[2], record_amount, record_size);
        } else{
            printf("You have passed wrong mode as an argument!\n");
            return -1;
        }
    }

    else if(strcmp(argv[1],"copy")==0){
        if(argc != 7){
            printf("You have not passed enough arguments!\n");
            return -1;
        }
        if(strcmp(argv[6], "sys") == 0){
            sys_copy(argv[2], argv[3], record_amount, record_size);
        }else if(strcmp(argv[6], "lib") == 0){
            lib_copy(argv[2], argv[3], record_amount, record_size);
        } else{
            printf("You have passed wrong mode as an argument!\n");
            return -1;
        }
    } else{
        printf("You have not specified the operation!\n");
        return -1;
    }

    getrusage(RUSAGE_SELF, &usage);
    gettimeofday(&real, 0);
    s_end = getTime(&usage.ru_stime);
    u_end = getTime(&usage.ru_utime);
    r_end = getTime(&real);

    printf("System: %*ldµs \nUser:   %*ldµs \nReal:   %*ldµs\n\n",
           10, (s_end - s_start),
           10, (u_end - u_start),
           10, (r_end - r_start));
/*
    printf("Hello, World!\n");
    lib_generate("data",5,10);
    printf("Goodbye, World!\n");
    print_file("data");
    printf("\n");
    lib_sort("data",5,10);
    print_file("data");
    lib_copy("data","data2",5,10);
    printf("\n");
    print_file("data2");
    lib_generate("data",5,10);
     */
    return 0;
}

