//
// Created by marek on 17.03.18.
//

#include "lib.h"

int lib_generate(char *file_path, int record_amount, int record_size){
    FILE *out_file = fopen(file_path, "w+");
    if(out_file == NULL)printf("ELO");
    FILE *in_file = fopen("/dev/urandom", "r");
    char *buffer = malloc(record_size * sizeof(char));
    int counter = 0;
    while(counter < record_amount){
        int data_read = (int)fread(buffer, sizeof(char), (size_t)record_size, in_file);
        if(data_read != record_size){
            printf("Error. Only %d bytes has been read by the buffor", data_read);
            return -1;
        }

        for(int j = 0; j < record_size; j++) {
            buffer[j] = (char) (abs(buffer[j]) % 25 + 97);
        }
        buffer[record_size-1] = 10;

        int data_wrote = (int)fwrite(buffer, sizeof(char), (size_t)record_size, out_file);
        if(data_wrote != record_size){
            printf("Error. %d bytes has been read, but only %d bytes has been written to the file", data_read,
                   data_wrote);
        }
        counter++;
    }
    fclose(in_file);
    fclose(out_file);
    free(buffer);
    return 1;
}

int lib_record_comparator(char *r1, char *r2){
    if(r1 == NULL || r2 == NULL) return 0;
    if(r1[0] > r2[0]) return 1;
    else if(r1[0] == r2[0]) return 0;
    else return -1;
}

int lib_sort(char *file_path, int record_amount, int record_size){
    FILE* file = fopen(file_path, "r+");
    if(file == NULL){
        printf("Opening the file failed.\n");
        return -1;
    }
    char* r1 = malloc(record_size * sizeof(char));
    char* r2 = malloc(record_size * sizeof(char));

    for(int i=0; i < record_amount; i++){

        fseek(file, i * record_size, SEEK_SET);
        fread(r1, sizeof(char), record_size * sizeof(char), file);
        for(int j=i; j-1 >=0; j--){
            fseek(file, (j-1) * record_size, SEEK_SET);
            fread(r2, sizeof(char), record_size * sizeof(char), file);
            if(lib_record_comparator(r1, r2) == -1){
                fseek(file, (-1) * record_size, 1);
                if(fwrite(r1, sizeof(char), record_size * sizeof(char), file) != record_size){
                    printf("Sorting failed");
                    return -1;
                }
                if(fwrite(r2, sizeof(char), record_size * sizeof(char), file) != record_size){
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
    fclose(file);
    return 1;
}

int lib_copy(char *source_path, char *destination_path, int record_amount, int record_size){
    FILE *source = fopen(source_path, "r");
    if(source == NULL){
        printf("Opening the file failed.\n");
        return -1;
    }
    FILE* destination = fopen(destination_path, "w+");
    char* buffer = malloc(record_size * sizeof(char));

    for(int i=0; i < record_amount; i++){
        if(fread(buffer, sizeof(char), record_size * sizeof(char), source) != record_size){
            printf("Copying failed1 %d", i);
            return -1;
        }
        if(fwrite(buffer, sizeof(char), record_size * sizeof(char), destination) != record_size){
            printf("Copying failed2 %d", i);
            return -1;
        }
    }
    fclose(source);
    fclose(destination);
    free(buffer);
    return 1;
}