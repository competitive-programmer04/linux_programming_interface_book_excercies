#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc, char *argv[]){
    if(argc < 3){
        fprintf(stderr, "Please pass the option as well as the file name\n");
        return -1;
    }
    else if(argc > 3){
        fprintf(stderr, "Too many arguments\n");
        return -1;
    }
    else{
        int opt = 0;
        char *file_path;
        while((opt = getopt(argc, argv, "a:")) != -1){
            switch(opt){
                case 'a':{
                             file_path = optarg;
                             break;
                         }
                default:{
                            fprintf(stderr, "Invalid option\n");
                            break;
                        }
            }
        }
        if(!file_path){
            fprintf(stderr, "Pass option a as well as the file name\n");
            return -1;
        }
        int fd = open(file_path, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
        if(fd == -1){
            perror("open");
            return -1;
        }
        char buf[1025];
        ssize_t bytes_read = 0;
        while((bytes_read = read(STDIN_FILENO, buf, sizeof(buf)-1)) != 0){
            if(bytes_read == -1){
                perror("read");
                close(fd);
                return -1;
            }
            buf[bytes_read] = '\0';
            if(write(STDOUT_FILENO, buf, bytes_read) == -1){
                perror("write");
                close(fd);
                return -1;
            }
            if(write(fd, buf, bytes_read) == -1){
                perror("write");
                close(fd);
                return 1;
            }
        }
        fprintf(stdout, "data has been successfully written to both stdout and the %s\n", file_path);
        close(fd);
        return 0;
    }
}
