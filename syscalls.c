// decided to include sycall.h dependencies again for self-contained headers & compile-time checks
#include <stdio.h>    /* for fprintf, fread, fwrite, fseek, ftell, fclose */
#include <dirent.h>   // for DIR
#include <sys/stat.h> // for stat struct
#include <stdlib.h>   /* for exit */
#include <unistd.h>   /* for getcwd */
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
        fprintf(stderr, "Stat error on entry '%s' (%d): %s\n", path, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return 0;
}

FILE *Fopen(const char *path, const char *mode)
{
    FILE *fp = fopen(path, mode);
    if (fp == NULL)
    {
        fprintf(stderr, "Fopen error on file '%s' (%d): %s\n", path, errno, strerror(errno));
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

void *Malloc(size_t size)
{
    void *voidptr = malloc(size);
    if (voidptr == NULL)
    {
        fprintf(stderr, "Malloc failed when attempting to allocate %zu bytes.\n", size);
        fprintf(stderr, "Error (%d): %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return voidptr;
}

char *Strdup(const char *s)
{
    char *buf = strdup(s);
    if (buf == NULL)
    {
        fprintf(stderr, "Strdup error (%d): %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return buf;
}

char *Getcwd(void)
{
    char *buf = getcwd(NULL, 0); // dynamic allocation for cwd, must free
    if (buf == NULL)
    {
        fprintf(stderr, "Getcwd error (%d): %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return buf;
}