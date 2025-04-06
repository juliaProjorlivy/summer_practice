#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    const char *filename = "chat.fifo";

    if(access(filename, F_OK) == -1)
    {
        (void)umask(0);
        if(mkfifo(filename, 0666) == -1)
        {
            printf("Failed to create FIFO\n");
            return 1;
        }
    }

    int fd = 0;
    if ((fd = open(filename, O_WRONLY)) < 0)
    {
        printf("Failed to open FIFO\n");
        return 1;
    }

    int val = 0;
    while(val < 100)
    {
        ssize_t size = write(fd, &val, sizeof(int));
        if(size <= 0)
        {
            printf("Failed to write integer\n");
            return 1;
        }
        printf("\"%s\" has written %d\n", argv[0], val);
        ++val;
    }

    close(fd);
    return 0;
}

