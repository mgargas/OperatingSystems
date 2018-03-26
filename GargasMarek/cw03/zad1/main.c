#define __USE_XOPEN_EXTENDED
#define _GNU_SOURCE
#define __USE_OPENE
#include <stdio.h>
#include <time.h>
#include <dirent.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <memory.h>
#include <ftw.h>
#include <sys/types.h>
#include <unistd.h>

char program_operator;
time_t program_reference_time;

void set_program_operator(char operator){
    program_operator = operator;
}
void set_program_reference_time(time_t reference_time){
    program_reference_time = reference_time;
}

// 1 if conditions are met, otherwise -1
int check_time(time_t modification_time){
    double difference = difftime(modification_time, program_reference_time);
    if(program_operator == '=' && difference == 0) return 1;
    else if(program_operator == '<' && difference < 0) return 1;
    else if(program_operator == '>' && difference > 0) return 1;
    return -1;
}

static int print_info(const char* path, const struct stat* statistics, int type_flag, struct FTW* ftbuf){
	
	//printf("%s",path);
    if(S_ISREG(statistics->st_mode) && (check_time(statistics->st_mtime) == 1)) {

        //printf("Modification time: ");
        char *time = ctime(&statistics->st_mtime);
        time[strlen(time) - 1] = '\0';
        printf("%s",time);
        printf(" ");

        //printf("File Permissions: ");
        printf((S_ISDIR(statistics->st_mode)) ? "d" : "-");
        printf((statistics->st_mode & S_IRUSR) ? "r" : "-");
        printf((statistics->st_mode & S_IWUSR) ? "w" : "-");
        printf((statistics->st_mode & S_IXUSR) ? "x" : "-");
        printf((statistics->st_mode & S_IRGRP) ? "r" : "-");
        printf((statistics->st_mode & S_IWGRP) ? "w" : "-");
        printf((statistics->st_mode & S_IXGRP) ? "x" : "-");
        printf((statistics->st_mode & S_IROTH) ? "r" : "-");
        printf((statistics->st_mode & S_IWOTH) ? "w" : "-");
        printf((statistics->st_mode & S_IXOTH) ? "x" : "-");
        printf(" ");


        //size
        //printf("Size: ");
        printf("%d", (int) statistics->st_size);
        printf(" ");

        //actual path
        char actual_path[PATH_MAX + 1];
        char *ptr;
        ptr = realpath(path, actual_path);
        //printf("Path: ");
        printf("%s", ptr);
        printf("\n");
    }

    return 0;
}


void print_directory(const char* path){
    DIR* dir_stream = opendir(path);
    if(dir_stream == NULL){
        printf("Enable to get to the directory");
    }

    struct dirent* file;
    struct stat* statistics = malloc(sizeof(struct stat));
    char path_buff[PATH_MAX+1];
    while((file = readdir(dir_stream)) != NULL){

        strcpy(path_buff, path);
        strcat(path_buff, "/");
        strcat(path_buff, file->d_name);

        if(strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, ".." ) == 0) continue;

        if(lstat(path_buff, statistics) >= 0) {

            if(S_ISDIR(statistics->st_mode)){
                pid_t child_pid;
                //child_pid = fork();
                if((child_pid = fork()) == 0)
                {
                    //printf("Child process: Parent process pid:%d\n",(int)getppid());
                    //printf("Child process: Child process pid:%d\n",(int)getpid());
                    print_directory(path_buff);
                    exit(0);
                }

            }
            else if(S_ISREG(statistics->st_mode)){

                print_info(path_buff, statistics, FTW_F, NULL);
            }

        }
    }
    closedir(dir_stream);
}



int main(int argc, char *argv[]) {
    
    char* info = "Arguments should be: [stat|ntfw] [path] ['<'|'='|'>'] [YYYY-mm-dd] [HH:MM:SS]";
    if(argc != 6){
        printf("You have passed wrong number of arguments!\n");
        printf("%s", info);
        return -1;
    }
    set_program_operator(argv[3][0]);
 
    if((strcmp(argv[1],"stat") == 0 || strcmp(argv[1],"nftw") == 0)) {

        char argument_time[20];
        strcpy(argument_time, argv[4]);
        argument_time[10] = ' ';
        strcat(argument_time, argv[5]);
        argument_time[19] = '\0';
        struct tm tm;
        strptime(argument_time, "%Y-%m-%d %H:%M:%S", &tm);
        set_program_reference_time(mktime(&tm));
    
        if(strcmp(argv[1],"stat") == 0) print_directory(argv[2]);
        else nftw(argv[2],print_info,10,FTW_PHYS);
    }

        return 0;
}
