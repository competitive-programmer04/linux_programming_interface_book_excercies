#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int write_data(int fd, char *my_bytes){
    size_t num_bytes = 0;
    int len = strlen(my_bytes);
    for(int  i=0; i<len; ++i){
        size_t mul = 1;
        for(int j=1; j<len-i-1; ++j){
            mul = mul*10;
        }
        num_bytes = num_bytes + (my_bytes[i] - '0')*mul;
    }
    char my_buf[1000001];
    for(size_t  i=0; i<sizeof(my_buf)-1; ++i){
        my_buf[i] = (i-'0')%256;
    }
    my_buf[sizeof(my_buf)-1] = '\0';
    if(write(fd, my_buf, sizeof(my_buf)-1) != sizeof(my_buf)-1){
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[]){
    if(argc < 3 || argc > 4){
        fprintf(stderr, "Wrong format of input\n");
        exit(1);
    }
    char *file_path = argv[1];
    if(argc < 4){
        int fd = open(file_path, O_WRONLY|O_APPEND|O_CREAT, S_IRUSR|S_IWUSR);
        if(fd == -1){
            perror("open");
            exit(1);
        }
        if(write_data(fd, argv[2]) == -1){
            perror("write");
            close(fd);
            exit(1);
        }
        off_t size = lseek(fd, 0, SEEK_END);
        fprintf(stdout, "data has been written into the file and the new file size is %ld MB\n", size/1000000);
        close(fd);
    }
    else{
        int fd = open(file_path, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
        if(fd == -1){
            perror("open");
            exit(1);
        }
        if(lseek(fd, 0, SEEK_END) == -1){
            perror("lseek");
            close(fd);
            exit(1);
        }
        write_data(fd, argv[2]);
        off_t size = lseek(fd, 0, SEEK_END);
        fprintf(stdout, "data has been written into the file and the new file size id %ld MB\n", size/1000000);
        close(fd);
    }
    exit(0);
}
