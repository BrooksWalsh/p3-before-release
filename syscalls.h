#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <dirent.h>   // DIR type
#include <sys/stat.h> // stat struct
#include <stdio.h>    // FILE type

DIR *Opendir(const char *name);
struct dirent *Readdir(DIR *dirp);
int Closedir(DIR *dirp);
int Stat(const char *restrict path, struct stat *restrict buf);
FILE *Fopen(const char *path, const char *mode);
int Fclose(FILE *fp);
void *Malloc(size_t size);
char *Strdup(const char *s);
char *Getcwd(void);

#endif