#ifndef CINCLUDESFUNCS_H
#define CINCLUDESFUNCS_H

#include <sys/types.h> // for size_t

void parseArgs(int argc, char **argv, LogicManager *LogicManager);
void findIncludesInFile(char *filePath, FileIncludes **results, int *resultCount);
void findIncludesInDir(char *dirPath, FileIncludes **results, int *resultCount);
void sortResult(FileIncludes **results, LogicManager *LogicManager, int resultCount);
void printResult(FileIncludes **results, LogicManager *LogicManager, int resultCount);
void freeResult(FileIncludes **results, int resultCount);

#endif