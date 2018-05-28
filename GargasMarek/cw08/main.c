#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/times.h>
#include <zconf.h>

#define FAILURE_EXIT(code, format, ...) { printf(format, ##__VA_ARGS__); exit(code);}



unsigned char** I; //image
float** K; //filter
unsigned char** J; //output image with applied filter

int H; //image's height
int W; //image's width
int C; //image's biggest colour's value
int M; // filter's size
int threads_count;
int size; //H*W
int part_size; //size/thread_count


//MATH UTILS
int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a > b ? b : a;
}


//TIME MEASUREMENT
double time_diff(clock_t start, clock_t end) {
    return (double) (end - start) / sysconf(_SC_CLK_TCK);
}

void fprintf_time(FILE *file, clock_t rStartTime, struct tms tmsStartTime, clock_t rEndTime, struct tms tmsEndTime) {
    fprintf(file, "Real:   %.2lf s   ", time_diff(rStartTime, rEndTime));
    fprintf(file, "User:   %.2lf s   ", time_diff(tmsStartTime.tms_utime, tmsEndTime.tms_utime));
    fprintf(file, "System: %.2lf s\n", time_diff(tmsStartTime.tms_stime, tmsEndTime.tms_stime));
}

void save_times(clock_t r_time[2], struct tms tms_time[2], char* picture_file_path,  char* filter_file_path) {
    FILE *file;
    if ((file = fopen("Times.txt", "a")) == NULL) FAILURE_EXIT(2, "Opening times file failed");
    fprintf(file, "Input image: %s Filter: %s \n", picture_file_path, filter_file_path);
    fprintf(file, "Number of Threads: %d\n", threads_count);
    fprintf(file, "Picture size: %dx%d\n", W, H);
    fprintf(file, "Filter size: %dx%d\n", C, C);
    fprintf_time(file, r_time[0], tms_time[0], r_time[1], tms_time[1]);
    fprintf(file, "\n\n");
    fclose(file);
}



//FILE PARSING
void parse_image(char* path) {
    FILE* file = fopen(path, "r");
    fscanf(file, "P2 %d %d %d", &W, &H, &M);

    I = calloc(H, sizeof(unsigned char*));
    J = calloc(H, sizeof(unsigned char*));

    for (int i = 0; i < H; ++i) {
        I[i] = calloc(W, sizeof(unsigned char));
        J[i] = calloc(W, sizeof(unsigned char));
        for (int j = 0; j < W; ++j) {
            fscanf(file, "%d ", &I[i][j]);
        }
    }
    fclose(file);
}


void parse_filter(char* path) {
    FILE* file = fopen(path, "r");
    fscanf(file, "%d\n", &C);

    K = calloc(C, sizeof(float*));
    for (int i = 0; i < C; ++i) {
        K[i] = calloc(C, sizeof(float));
        for (int j = 0; j < C; ++j) {
            fscanf(file, "%f ", &K[i][j]);
        }
    }

    fclose(file);
}


void save_file(char* path) {
    FILE* file = fopen(path, "w");
    fprintf(file, "P2\n%d %d\n%d\n", W, H, M);

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            fprintf(file, "%d ", J[i][j]);
        }
    }
    fprintf(file, "\n");
}


// DATA PROCESSING
int process_pixel(int x, int y) {
    double sum = 0;
    int c2 = (int) floor(C / 2);
    for (int h = 0; h < C; ++h) {
        for (int w = 0; w < C; ++w) {
            int i_y = min((H - 1), max(0, y - c2 + h));
            int i_x = min((W - 1), max(0, x - c2 + w));
            sum += I[i_y][i_x] * K[h][w];
        }
    }
    return (int) max(0, min(round(sum), 255));
}


void *process_picture_part(void *args){
    int thread_number = *(int *) args;
    //printf("nr: %d ", thread_number);
    int start = thread_number*part_size;
    int end =  thread_number != threads_count-1 ? start + part_size : size;
    //printf(" %d %d\n", start, end);
    for(int i = start; i < end; i++){
        J[i/W][i%W] = process_pixel(i%W, i/W);
    }
    return NULL;
}


void apply_filter(){
    size = W*H;
    part_size = size/threads_count;
    pthread_t *thread = malloc(threads_count * sizeof(pthread_t));

    for(int i=0; i < threads_count; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&thread[i], NULL, process_picture_part, arg);
    }

    for(int i=0; i < threads_count; i++){
        pthread_join(thread[i], 0);
    }
}



int main(int argc, char* argv[]) {

    //parse arguments
    if (argc < 5) FAILURE_EXIT(1, "./filter <thread_number> <picture_file_path> <filter_file_path> <result_file_path>");
    if ((threads_count = (int) strtol(argv[1], NULL, 10)) <= 0) FAILURE_EXIT(2, "thread_number must be grater than 0");
    char* picture_file_path = argv[2];
    char* filter_file_path = argv[3];
    char* result_file_path = argv[4];


    //initialise time structures
    clock_t r_time[2] = {0, 0};
    struct tms tms_time[2];

    //prepare files
    parse_image(picture_file_path);
    parse_filter(filter_file_path);

    //change image I into image J with filter K and measure the time
    r_time[0] = times(&tms_time[0]);
    apply_filter();
    r_time[1] = times(&tms_time[1]);

    save_file(result_file_path);
    save_times(r_time, tms_time, picture_file_path, filter_file_path);

    return 0;
}
