#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <memory.h>

void print_file_id(int file_descriptor) {
    struct stat info;
    if (fstat(file_descriptor, &info) != 0)
        fprintf(stderr, "stat() error for file_descriptor %d: %s\n", file_descriptor,
                strerror(errno));
    else
        printf("The file id of file_descriptor %d is %d\n", file_descriptor,
               (int) info.st_ino);
}

int main() {
    int file_descriptor, file_descriptor2;
    char fn[] = "original.file.txt";
    char fn2[] = "dup2.file";

    /* create original file */
    if((file_descriptor = creat(fn, S_IRUSR | S_IWUSR)) < 0)
        perror("creat() error");
        /* create file to dup to */
    else if((file_descriptor2 = creat(fn2, S_IWUSR)) < 0)
        perror("creat()error");
        /* dup file_descriptor to file_descriptor2; print results */
    else {
        print_file_id(file_descriptor);
        print_file_id(file_descriptor2);
        if ((file_descriptor2 = dup2(file_descriptor, file_descriptor2)) < 0)
            perror("dup2() error");
        else {
            puts("After dup2()...");
            print_file_id(file_descriptor);
            print_file_id(file_descriptor2);
            puts("The file descriptors are different but they");
            puts("point to the same file which is different than");
            puts("the file that the second file_descriptor originally pointed to.");
            FILE * stream = fdopen(file_descriptor2, "w");
            fprintf(stream, "soner to fd2 but it points to fd original.file.txt  from now on \n");
            close(file_descriptor);
            //close(file_descriptor2);
            fclose(stream);
        }
        //unlink(fn);
        unlink(fn2);
    }
    return 0;
}
