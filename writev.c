#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/uio.h>
#include<unistd.h>
#include<fcntl.h>

ssize_t my_writev(int fd, struct iovec *iov, int iovcount){
    size_t len =0;
    for(int i=0; i<iovcount; ++i){
        len = len + iov[i].iov_len - 1;
    }
    char large_buffer[len + 1];
    size_t prev_bytes_write = 0;
    for(int i=0; i<iovcount; ++i){
        memcpy(large_buffer + prev_bytes_write, iov[i].iov_base, iov[i].iov_len);
        prev_bytes_write = prev_bytes_write + iov[i].iov_len - 1;
    }
    large_buffer[len] = '\0';
    ssize_t bytes_write = write(fd, large_buffer, len);
    if(bytes_write == -1){
        return -1;
    }
    return bytes_write;
}

int main(int argc, char *argv[]){
    if(argc == 1){
        fprintf(stderr, "Please pass the file name to read\n");
        exit(1);
    }
    else if(argc > 2){
        fprintf(stderr, "Too many arguments\n");
        exit(1);
    }
    else{
        ssize_t bytes_write = 0;
        ssize_t total_bytes = 0;
        int iovcount = 3;
        struct iovec iov[3];
        size_t buf1_size = 1024;
        size_t buf2_size = 1024;
        size_t buf3_size = 1024;
        char buf1[buf1_size];
        for(size_t i=0; i<sizeof(buf1)-1; ++i){
            buf1[i] = 'a';
        }
        buf1[buf1_size-1] = '\0';
        char buf2[buf2_size];
        for(size_t i=0; i<sizeof(buf2)-1; ++i){
            buf2[i] = 'b';
        }
        buf2[buf2_size-1] = '\0';
        char buf3[buf3_size];
        for(size_t i=0; i<sizeof(buf3)-1; ++i){
            buf3[i] = 'c';
        }
        buf3[buf3_size-1] = '\0';
        iov[0].iov_base = buf1;
        iov[0].iov_len = buf1_size;
        total_bytes = total_bytes + buf1_size -1 ;
        iov[1].iov_base = buf2;
        iov[1].iov_len = buf2_size;
        total_bytes = total_bytes + buf2_size -1 ;
        iov[2].iov_base = buf3;
        iov[2].iov_len = buf3_size;
        total_bytes = total_bytes + buf3_size -1 ;
        char *file_path = argv[1];
        int fd = open(file_path, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
        if(fd == -1){
            perror("open");
            exit(1);
        }
        bytes_write = my_writev(fd, iov, iovcount);
        if(bytes_write == -1){
            perror("write");
            close(fd);
            exit(1);
        }
        fprintf(stdout, "%lu bytes written into the file: %s and requested: %lu bytes\n", bytes_write, file_path, total_bytes);
        close(fd);
        exit(0);
    }
}
