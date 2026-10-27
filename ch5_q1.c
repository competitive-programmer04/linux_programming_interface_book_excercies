#define _FILE_OFFSET_BITS 64
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[]){
    if(argc == 3){
        char *file_path = argv[1];
        int fd = open(file_path, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
        if(fd == -1){
            perror("open");
            exit(1);
        }
        off_t offset = 0;
        char *my_offset = argv[2];
        int len = strlen(my_offset);
        for(int i=0; i<len; ++i){
            off_t mul = 1;
            for(int j=1; j<=len-i-1; ++j){
                mul = mul*10;
            }
            offset = offset + (my_offset[i] - '0')*mul;
        }
        if(lseek(fd, offset, SEEK_SET) == -1){
            close(fd);
            perror("lseek");
        }
        if(write(fd, "testing", 7) != 7){
            close(fd);
            perror("write");
        }
        fprintf(stdout, "data has been written into the file %s at offset %ld", file_path, offset);
        close(fd);
    }
    else{
        fprintf(stderr, "Wrong input\n");
        exit(1);
    }
    exit(0);
}
