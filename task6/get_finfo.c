#include <stdio.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Too little arguments. Povide file name\n");
        return 1;
    }
    const char *filename = argv[1];
    struct stat finfo;
    if(stat(argv[1], &finfo) == -1)
    {
        perror("Failed to stat file");
        return 1;
    }

    printf("File type:                ");
    switch (finfo.st_mode & S_IFMT) {
        case S_IFBLK:  printf("block device\n");            break;
        case S_IFCHR:  printf("character device\n");        break;
        case S_IFDIR:  printf("directory\n");               break;
        case S_IFIFO:  printf("FIFO/pipe\n");               break;
        case S_IFLNK:  printf("symlink\n");                 break;
        case S_IFREG:  printf("regular file\n");            break;
        case S_IFSOCK: printf("socket\n");                  break;
        default:       printf("unknown?\n");                return 1;
    }

    printf("ID of containing device:  [%x,%x]\n",
           major(finfo.st_dev),
           minor(finfo.st_dev));

    printf("I-node number:            %ju\n", (uintmax_t) finfo.st_ino);

    printf("Mode:                     %jo (octal)\n",
           (uintmax_t) finfo.st_mode);

    printf("Link count:               %ju\n", (uintmax_t) finfo.st_nlink);
    printf("Ownership:                UID=%ju   GID=%ju\n",
           (uintmax_t) finfo.st_uid, (uintmax_t) finfo.st_gid);

    printf("Preferred I/O block size: %jd bytes\n",
           (intmax_t) finfo.st_blksize);
    printf("File size:                %jd bytes\n",
           (intmax_t) finfo.st_size);
    printf("Blocks allocated:         %jd\n",
           (intmax_t) finfo.st_blocks);

    printf("Last status change:       %s", ctime(&finfo.st_ctime));
    printf("Last file access:         %s", ctime(&finfo.st_atime));
    printf("Last file modification:   %s", ctime(&finfo.st_mtime));

    return 0;
}


