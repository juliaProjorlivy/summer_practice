#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

int read_file(int fd, size_t size)
{
    pid_t pid = getpid();

    // The size of the file might be too big -> using dymanic allocation
    char *new_buf = (char *)calloc(sizeof(char), size + 1);
    if(!new_buf)
    {
        fprintf(stderr, "pid %d: Failed to allocate memroy", pid);
        return 1;
    }
    int n_read = read(fd, new_buf, size);
    if(n_read < 0)
    {
        fprintf(stderr, "pid %d: Failed to read from file", pid);
        return 1;
    }
    new_buf[n_read] = '\0';
    printf("from process %d: %s\n", pid, new_buf);
    free(new_buf);
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Too little arguments. Provide filename.\n");
        return 1;
    }

    const char *filename = argv[1];
    int fd = 0;
    if((fd = open(filename, O_RDONLY)) == -1)
    {
        perror("Failed to open file");
        return 1;
    }

    // To get file size
    struct stat finfo;
    if(stat(filename, &finfo) == -1)
    {
        perror("Failed to stat file");
        close(fd);
        return 1;
    }

    // Buffer containing file content
    char *buf = (char *)calloc(sizeof(char), finfo.st_size);
    if(!buf)
    {
        perror("Failed to allocate memory");
        close(fd);
        return 1;
    }
    if(read(fd, buf, finfo.st_size) != finfo.st_size)
    {
        perror("Failed to read to new file");
        close(fd);
        return 1;
    }
    close(fd);

    // Fork into two processes
    pid_t pid = 0;
    if((pid = fork()) < 0)
    {
        perror("Failed to fork");
        return 1;
    }

    // Creating output file name
    char new_filename[100] = {};
    if(sprintf(new_filename, "new_%d", getpid()) < 0)
    {
        fprintf(stderr, "Failed to make a filename %s\n", new_filename);
        return 1;
    }

    // Open output file on writing
    int new_fd = 0;
    if((new_fd = open(new_filename, O_CREAT | O_RDWR, 0666)) == -1)
    {
        fprintf(stderr, "Failed to open new file %s\n", new_filename);
        free(buf);
        return 1;
    }

    // Write to output file from buf
    if(write(new_fd, buf, finfo.st_size) != finfo.st_size)
    {
        fprintf(stderr, "Failed to write to new file %s\n", new_filename);
        free(buf);
        close(new_fd);
        return 1;
    }

    // Rewind to start
    lseek(new_fd, 0, SEEK_SET);

    // Parent process
    if(pid)
    {
        // Wait for child to terminate
        waitpid(pid, NULL, 0);
    }
    read_file(new_fd, finfo.st_size);

    free(buf);
    close(new_fd);
    return 0;
}

