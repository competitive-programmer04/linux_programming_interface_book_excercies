#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <errno.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int my_dup(int old_fd){
    int flags = fcntl(old_fd, F_GETFL);
    if(flags == -1){
        fprintf(stderr, "%d is invalid file descriptor\n", old_fd);
        errno = EBADF;
        return -1;
    }
    int new_dup_fd = fcntl(old_fd, F_DUPFD, 0);
    if(new_dup_fd == -1){
        perror("my_dup");
        return -1;
    }
    return new_dup_fd;
}

int my_dup2(int old_fd, int new_fd){ 
    int flags = fcntl(old_fd, F_GETFL);
    if(flags == -1){
        fprintf(stderr, "%d is not a valid file descriptor\n", old_fd);
        errno = EBADF;
        return -1;
    }
    flags = fcntl(new_fd, F_GETFL);
    if(flags != -1){
        if(close(new_fd) == -1){
            perror("close");
            return -1;
        }
    }
    new_fd = fcntl(old_fd, F_DUPFD, new_fd);
    if(new_fd == -1){
        perror("my_dup2");
        return -1;
    }
    return new_fd;
}

int main(int argc, char *argv[]){
    if(argc == 2){
        char *ptr = argv[1];
        int old_fd = *ptr-'0';
        int new_fd = my_dup(old_fd);
        if(new_fd != -1){
            fprintf(stdout, "dup successfully implemented\n");
            fprintf(stdout, "old fd: %d & new fd: %d\n", old_fd, new_fd);
        }
    }
    else if(argc == 3){
        char *ptr1 = argv[1];
        int old_fd = *ptr1 - '0';
        char *ptr2 = argv[2];
        int new_fd = *ptr2 - '0';
        new_fd = my_dup2(old_fd, new_fd);
        if(new_fd != -1){
            fprintf(stdout, "dup2 successfully implemented\n");
            fprintf(stdout, "old fd: %d & new fd: %d\n", old_fd, new_fd);
        }
    }
    else{
        fprintf(stderr, "Wrong input\n");
        return -1;
    }
    return 0;
}
