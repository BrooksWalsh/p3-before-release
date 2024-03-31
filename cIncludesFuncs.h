#ifndef CINCLUDESFUNCS_H
#define CINCLUDESFUNCS_H

int parseArgs(int argc, char **argv, LogicManager *LogicManager);
void findIncludesInDir(char *dirPath, FileIncludes *results);
void findIncludesInFile(char *filePath, FileIncludes *results);
void sortResult(FileIncludes *results, LogicManager *LogicManager);
void printResult(FileIncludes *results);

#endif