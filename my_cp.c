#include<fcntl.h>
#include <stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>

int main(int argc, char *argv[]){
    if(argc < 3){
        fprintf(stderr, "Please pass the name of both files\n");
        return EXIT_FAILURE;
    }
    else if(argc > 3){
        fprintf(stderr, "Too many arguments\n");
        return EXIT_FAILURE;
    }
    else{
        int input_fd, output_fd, output_flag;
        mode_t output_perm;
        const char *inp_file_path = argv[1];
        input_fd = open(inp_file_path, O_RDONLY);
        if(input_fd == -1){
            perror("input file");
            return EXIT_FAILURE;
             }
        output_flag = O_CREAT | O_WRONLY | O_TRUNC;
        output_perm = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;  // rw-rw-r
        const char *out_file_path = argv[2];
        output_fd = open(out_file_path, output_flag, output_perm);
        if(output_flag == -1){
            perror("output file");
            close(input_fd);
            return EXIT_FAILURE;
        }
        char line_buf[1025]; // 0-1023(1024 charcters) for file and 1024th index for \0
        ssize_t bytes_read = 0;
        while((bytes_read = read(input_fd, line_buf, sizeof(line_buf)-1)) > 0){
            line_buf[bytes_read] = '\0';
            if(write(output_fd, line_buf, bytes_read) != bytes_read){
                fprintf(stderr, "error occured while writing the data to the output file\n");
                close(input_fd);
                close(output_fd);
                return EXIT_FAILURE;
            }
        }
        if(bytes_read == -1){
            fprintf(stderr, "error occurred while reading the data from the input file\n");
            close(input_fd);
            close(output_fd);
            return EXIT_FAILURE;
        }
        close(input_fd);
        close(output_fd);
        fprintf(stdout, "%s is successfully copied to %s\n", inp_file_path, out_file_path);
        return EXIT_SUCCESS;
    }
}
