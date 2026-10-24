#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/uio.h>
#include<unistd.h>
#include<fcntl.h>

ssize_t my_readv(int fd, struct iovec *iov, int iovcount){
    size_t len =0;
    for(int i=0; i<iovcount; ++i){
        len = len + iov[i].iov_len - 1;
    }
    char large_buffer[len + 1];
    memset(large_buffer, 0, len);
    ssize_t bytes_read = read(fd, large_buffer, len);
    if(bytes_read == -1){
        return -1;
    }
    large_buffer[bytes_read] = '\0';
    size_t prev_bytes_read = 0;
    for(int i=0; i<iovcount; ++i){
        memcpy(iov[i].iov_base, large_buffer + prev_bytes_read, iov[i].iov_len-1);
        char *my_buf = iov[i].iov_base;
        my_buf[iov[i].iov_len - 1] = '\0';
        prev_bytes_read = prev_bytes_read + iov[i].iov_len - 1;
    }
    return bytes_read;
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
        ssize_t bytes_read = 0;
        ssize_t total_bytes = 0;
        int iovcount = 3;
        struct iovec iov[3];
        size_t buf1_size = 1024;
        size_t buf2_size = 1024;
        size_t buf3_size = 1024;
        char buf1[buf1_size];
        char buf2[buf2_size];
        char buf3[buf3_size];
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
        int fd = open(file_path, O_RDONLY);
        if(fd == -1){
            perror("open");
            exit(1);
        }
        bytes_read = my_readv(fd, iov, iovcount);
        if(bytes_read == -1){
            perror("read");
            close(fd);
            exit(1);
        }
        fprintf(stdout, "%lu bytes readed from the file: %s\n", bytes_read, file_path);
        if(bytes_read < total_bytes){
            fprintf(stdout, "fewer bytes available in the file: %s then bytes requested\n", file_path);
        }
        else if(bytes_read == total_bytes){
            fprintf(stdout, "bytes read: %lu and bytes requested: %lu\n", bytes_read, total_bytes);
        }
        close(fd);
        exit(0);
    }
}
