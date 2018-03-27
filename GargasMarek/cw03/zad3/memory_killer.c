#include <stdlib.h>
#include <stdio.h>

int ARR_SIZE = 1000000; 

int main() {
    int *arr = malloc(sizeof(int) * ARR_SIZE); //allocate 4MB of memory
    for (int i = 0; i< ARR_SIZE; i++) {
        arr[i] = '$';
    }
    return 0;
}
