//
// Created by marek on 17.03.18.
//

#include <memory.h>
#include "sys.h"
int sys_generate(char *file_path, int record_amount, int record_size){
    int out_file = open(file_path, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR);
    int in_file = open("/dev/urandom", O_RDONLY);
    char* buffer = malloc(record_size* sizeof(char));
    int counter = 0;
    while(counter < record_amount) {
        int data_read = (int)read(in_file, buffer, record_size * sizeof(char));
        for(int j = 0; j < record_size; j++)
            buffer[j] = (char) (abs(buffer[j])%25 + 97);
        buffer[record_size-1] = 10;
        /*printf("Read data %d ", data_read);
        printf(" Len: %d ", (int)strlen(buffer));
        printf(buffer);*/
        if (data_read < record_size) {
            printf("Error. Only %d bytes has been read by the buffor", data_read);
        }
        int data_wrote = (int)write(out_file, buffer, record_size * sizeof(char));
        //printf(" Wrote data %d \n", data_wrote);
        if (data_wrote < data_read) {
            printf("Error. %d bytes has been read, but only %d bytes has been written to the file", data_read,
                   data_wrote);
        }
        counter++;
    }
    free(buffer);
    close(in_file);
    close(out_file);
    return 1;
}

int sys_record_comparator(char *r1, char *r2){
    if(r1 == NULL || r2 == NULL) return 0;
    if(r1[0] > r2[0]) return 1;
    else if(r1[0] == r2[0]) return 0;
    else return -1;
}

int sys_sort(char *file_path, int record_amount, int record_size){
    int file = open(file_path, O_RDWR);
    if(file < 0){
        printf("Opening the file failed.\n");
        return -1;
    }
    char* r1 = malloc(record_size * sizeof(char));
    char* r2 = malloc(record_size * sizeof(char));

    for(int i=0; i < record_amount; i++){

        lseek(file, i * record_size, SEEK_SET);
        read(file, r1, record_size * sizeof(char));
        for(int j=i; j-1 >=0; j--){
            lseek(file, (j-1) * record_size, SEEK_SET);
            read(file, r2, record_size * sizeof(char));
            if(sys_record_comparator(r1, r2) == -1){
                lseek(file, (-1) * record_size, SEEK_CUR);
                if(write(file, r1, record_size * sizeof(char)) != record_size){
                    printf("Sorting failed");
                    return -1;
                }
                if(write(file, r2, record_size * sizeof(char)) != record_size){
                    printf("Sorting failed");
                    return -1;
                }
            } else{
                break;
            }
        }

    }
    free(r1);
    free(r2);
    close(file);
    return 1;
}

void print_file(char* file_path){
    char* buffer = malloc(10* sizeof(char));
    int we = open("data",O_RDONLY);
    while(read(we,buffer,10) == 10){
        for(int i=0;i<10;i++){
            printf("%d ", buffer[i]);
        }
        printf("\n");
    }
}
int sys_copy(char *source_path, char *destination_path, int record_amount, int record_size){
    int source = open(source_path, O_RDONLY);
    int destination = open(destination_path, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    char* buffer = malloc(record_size * sizeof(char));

    for(int i=0; i < record_amount; i++){
        if(read(source, buffer, record_size * sizeof(char)) != record_size){
            printf("Copying failed1 %d", i);
            return -1;
        }
        if(write(destination, buffer, record_size * sizeof(char)) != record_size){
            printf("Copying failed2 %d", i);
            return -1;
        }
    }
    close(source);
    close(destination);
    free(buffer);
    return 1;
}