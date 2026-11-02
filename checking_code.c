#define _GNU_SOURCE

#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>

int check_file_cnt = 0;

int main(int argc, char *argv[]){
    if(argc < 3){
        fprintf(stderr, "Also pass the directory path\n");
        return EXIT_FAILURE;
    }
    else if(argc > 3){
        fprintf(stderr, "Too many arguments\n");
        return EXIT_FAILURE;
    }
    else{
        const char *dir_path = argv[1];
        DIR *dir = opendir(dir_path);
        if(dir == NULL){
            perror("opendir");
            return EXIT_FAILURE;
        }
        struct dirent *dir_entry = NULL;
        while((dir_entry = readdir(dir)) != NULL){
            if(dir_entry->d_type == DT_REG){
                char file_path1[512];
                snprintf(file_path1, sizeof(file_path1), "%s/%s", dir_path, dir_entry->d_name);
                int fd1 = open(file_path1, O_RDONLY);
                if(fd1 == -1){
                    perror("open for reading");
                    return EXIT_FAILURE;
                }
                const char *file_path2 = argv[2];
                int fd2 = 0;
                if(check_file_cnt == 0){
                    fd2 = open(file_path2, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR);
                    check_file_cnt++;
                }
                else{
                    fd2 = open(file_path2, O_WRONLY|O_APPEND);
                }
                if(fd2 == -1){
                    perror("open for writing");
                    close(fd1);
                }
                else{/*
                    char my_buf[1025];
                    my_buf[0] = '\0';
                    ssize_t bytes_read = 0;
                    while((bytes_read = read(fd1, my_buf, sizeof(my_buf)-1)) != 0){
                        if(bytes_read == -1){
                            perror("read");
                            close(fd1);
                            close(fd2);
                            break;
                        }
                        my_buf[bytes_read] = '\0';
                        if(write(fd2, my_buf, bytes_read) == -1){
                            perror("write");
                            close(fd1);
                            close(fd2);
                            break;
                        }
                    }
                    close(fd1);
                    close(fd2);
                    */
                    off_t start = lseek(fd1, 0, SEEK_SET);
                    off_t end = lseek(fd1, 0, SEEK_END);
                    if(end - start > 0){
                        if(write(fd2, dir_entry->d_name, strlen(dir_entry->d_name)) == -1){
                            perror("write");
                            close(fd1);
                            close(fd2);
                            break;
                        }
                    }
                    write(fd2, "\r\n", 2);
                    close(fd1);
                    close(fd2);
                }
            }
        }
        if(errno == EBADF){
            perror("readdir");
            return EXIT_FAILURE;
        }
        fprintf(stdout, "data has been successfully appended to the file\n");
    }
    return EXIT_SUCCESS;
}
