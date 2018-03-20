//
// Created by marek on 17.03.18.
//

#ifndef LAB2_SYS_H
#define LAB2_SYS_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int sys_generate(char *file_path, int record_amount, int record_size);
int sys_sort(char *file_path, int record_amount, int record_size);
int sys_copy(char *source_path, char *destination_path, int record_amount, int record_size);
void print_file(char* file_path);

#endif //LAB2_SYS_H
