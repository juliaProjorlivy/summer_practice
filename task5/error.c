#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main()
{
    const char *filename = "error.txt";

    if(open(filename, O_RDONLY, 0666) == -1)
    {
        perror("Failed to open file");
        fprintf(stderr, "Failed to open file %s: Error %d\n", filename, errno);
        // For older version of glibc
        // if(errno < sys_nerr)
        // {
        //     fprintf(stderr, "Failed to open file %s:%s\n", filename, sys_errlist[errno]);
        // }

        // Instead of sys_errlist
        fprintf(stderr, "Failed to open file %s: %s\n", filename, strerror(errno));

        return 1;
    }

}

