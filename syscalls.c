// decided to include sycall.h dependencies again for self-contained headers & compile-time checks
#include <stdio.h>    /* for fprintf, fread, fwrite, fseek, ftell, fclose */
#include <dirent.h>   // for DIR methods
#include <sys/stat.h> // for stat
#include <stdlib.h>   /* for exit */
#include <unistd.h>   /* for read, write */
#include <errno.h>    /* for errno */
#include <string.h>   /* for strerror */

DIR *Opendir(const char *name)
{
    DIR *dir = opendir(name);
    if (dir == NULL)
    {
        fprintf(stderr, "Opendir error on directory %s (%d): %s\n", name, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return dir;
}

struct dirent *Readdir(DIR *dirp)
{
    errno = 0; // reset errno before the call to ensure no fake errors
    struct dirent *entry = readdir(dirp);
    if (entry == NULL && errno != 0)
    {
        fprintf(stderr, "Readdir error (%d): %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return entry; // still could be NULL if at the end of DIR
}

int Closedir(DIR *dirp)
{
    if (closedir(dirp) == -1)
    {
        fprintf(stderr, "Closedir error (%d): %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return 0;
}

int Stat(const char *restrict path, struct stat *restrict buf)
{
    if (stat(path, buf) == -1)
    {
        fprintf(stderr, "Stat error on file/dir %s (%d): %s\n", path, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return 0;
}

FILE *Fopen(const char *path, const char *mode)
{
    FILE *fp = fopen(path, mode);
    if (fp == NULL)
    {
        fprintf(stderr, "Fopen error on file %s (%d): %s\n", path, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return fp;
}

int Fclose(FILE *fp)
{
    if (fclose(fp) == -1)
    {
        fprintf(stderr, "Fclose error (%d): %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return 0;
}