//
// Created by marek on 17.03.18.
//

#ifndef LAB2_LIB_H
#define LAB2_LIB_H

#include <stdio.h>
#include <stdlib.h>

int lib_generate(char *file_path, int record_amount, int record_size);
int lib_sort(char *file_path, int record_amount, int record_size);
int lib_copy(char *source_path, char *destination_path, int record_amount, int record_size);
#endif //LAB2_LIB_H
