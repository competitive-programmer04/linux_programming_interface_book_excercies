#include<stdio.h>
#include<fcntl.h>
#include <stdlib.h>
#include<unistd.h>

int main(int argc, char *argv[]){
    if(argc == 1){
        fprintf(stderr, "pass the filename\n");
        exit(1);
    }
    char *file_path = argv[1];
    int fd = open(file_path, O_WRONLY|O_APPEND);
    if(fd == -1){
        perror("open");
        exit(1);
    }
    if(lseek(fd, 0, SEEK_SET) == -1){
        perror("lseek");
        close(fd);
        exit(1);
    }
    if(write(fd, "testing", 7) == -1){
        perror("write");
        close(fd);
        exit(1);
    }
    fprintf(stdout, "data has been written to the starting of the new line in the file %s\n", file_path);
    close(fd);
    exit(0);
}
