#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char *argv[]){
    if(argc == 1){
        fprintf(stderr, "Please pass the required arguments\n");
        return -1;
    }
    else if(argc > 6){
        fprintf(stderr, "Too many arguments\n");
        return -1;
    }
    else{
        int fd = open(argv[1], O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
        if(fd == -1){
            perror("open");
            return -1;
        }
        for(int i=2; i<argc; ++i){
            char *str = argv[i];
            switch(str[0]){
                case 'r':{
                             int len = strlen(argv[i]) - 1; //skipping the first character
                             char *ptr = str + 1;
                             char my_arg[len+1];
                             memcpy(my_arg, ptr, len);
                             my_arg[len] = '\0';
                             ssize_t num_bytes = 0;
                             for(int i=0; i<len; ++i){
                                 int mul = 1;
                                 for(int j=1; j<=len-i-1; ++j){
                                     mul = mul*10;
                                 }
                                 num_bytes = num_bytes + ((my_arg[i] - '0')*mul);
                             }
                             char my_buf[num_bytes+1];
                             ssize_t bytes_read = read(fd, my_buf, num_bytes);
                             if(bytes_read < num_bytes){
                                 if(bytes_read == 0){
                                     fprintf(stdout, "We have reached end of file. So we cannot read %lu bytes\n", num_bytes);
                                 }
                                 else if(bytes_read == -1){
                                     perror("read");
                                     close(fd);
                                     return -1;
                                 }
                                 else{
                                     my_buf[bytes_read] = '\0';
                                     fprintf(stdout, "%lu bytes to read but available %lu bytes\n", bytes_read, num_bytes);
                                 }
                             }
                             else{
                                 my_buf[bytes_read] = '\0';
                                 fprintf(stdout, "successfully readed %lu bytes from the file %s\n", bytes_read, argv[1]);
                                 printf("%s\n", my_buf);
                             }
                             break;
                         }
                case 'R':{
                             int len = strlen(argv[i]) - 1;
                             char *ptr = str+1;
                             char my_arg[len+1];
                             memcpy(my_arg, ptr, len);
                             my_arg[len] = '\0';
                             ssize_t num_bytes = 0;
                             for(int i=0; i<len; ++i){
                                 int mul = 1;
                                 for(int j=1; j<len-i-1; ++j){
                                     mul = mul*10;
                                 }
                                 num_bytes = num_bytes + ((my_arg[i] - '0')*mul);
                             }
                             char my_buf[num_bytes+1];
                             ssize_t bytes_read = read(fd, my_buf, num_bytes);
                             if(bytes_read < num_bytes){
                                 if(bytes_read == 0){
                                     fprintf(stdout, "We have reached the end of the file %s. So we cannnot read %lu bytes\n", argv[1], num_bytes);
                                 }
                                 else if(bytes_read == -1){
                                     perror("read");
                                     close(fd);
                                     return -1;
                                 }
                                 else{
                                     my_buf[bytes_read] = '\0';
                                     fprintf(stdout, "%lu bytes have to read but %lu bytes available in the file %s\n", bytes_read, num_bytes, argv[1]);
                                 }
                             }
                             else{
                                 my_buf[bytes_read] = '\0';
                                 fprintf(stdout, "successfully readed %lu bytes from the file %s\n", bytes_read, argv[1]);
                                 for(int i=0; i<bytes_read; ++i){
                                     printf("0x%02X ", my_buf[i]);
                                 }
                                 printf("\n");
                             }
                             break;
                         }
                case 's':{
                             int len = strlen(argv[i]) - 1;
                             char *ptr = str + 1;
                             char my_arg[len+1];
                             memcpy(my_arg, ptr, len);
                             my_arg[len] = '\0';
                             off_t offset = 0;
                             for(int i=0; i<len; ++i){
                                 int mul = 1;
                                 for(int j=1; j<=len-i-1; ++j){
                                     mul = mul*10;
                                 }
                                 offset = offset + ((my_arg[i]-'0')*mul);
                             }
                             off_t new_offset = lseek(fd, offset, SEEK_SET);
                             if(new_offset == -1){
                                 perror("lseek");
                                 close(fd);
                                 return -1;
                             }
                             else{
                                 fprintf(stdout, "file offset has been moved to %lu from starting\n", offset);
                             }
                             break;
                         }
                case 'w':{
                             int len = strlen(argv[i]) - 1;
                             char *ptr = str + 1;
                             ssize_t bytes_write = write(fd, ptr, len);
                             if(bytes_write == -1){
                                 perror("write");
                                 close(fd);
                                 return -1;
                             }
                             else{
                                 fprintf(stdout, "%s have been written to the file %s\n", ptr, argv[1]);
                             }
                             break;
                         }
                default:{
                            fprintf(stderr, "Invalid argument\n");
                            break;
                        }
            }
        }
        close(fd);
        return 0;
    }
}
