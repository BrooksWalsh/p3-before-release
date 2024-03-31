#ifndef SYSCALLS_H
#define SYSCALLS_H

// include DIR/FILE type and stat definition in header
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>

DIR *Opendir(const char *name);
struct dirent *Readdir(DIR *dirp);
int Closedir(DIR *dirp);
int Stat(const char *restrict path, struct stat *restrict buf);
FILE *Fopen(const char *path, const char *mode);
int Fclose(FILE *fp);

#endif