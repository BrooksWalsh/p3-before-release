#include <string.h>
#include <stdlib.h>
#include "syscalls.h"
#include "cIncludesGlobals.h"

int isDirOrHeader(const char *arg)
{
    // check for directory or file
    struct stat entryStat;
    Stat(arg, &entryStat);
    if (S_ISDIR(entryStat.st_mode))
    {
        return 1;
    }
    else if (S_ISREG(entryStat.st_mode))
    {
        if (arg[strlen(arg) - 2] == '.' && arg[strlen(arg) - 1] == 'h')
            return 2;
        else
            return 0;
    }
    else
    {
        return 0;
    }
}

void setStart(const char *arg, LogicManager *LogicManager)
{
    switch (isDirOrHeader(arg))
    {
    case 1:
        LogicManager->startPlace = startDir;
        break;
    case 2:
        LogicManager->startPlace = startFile;
        break;
    }
}

int isValidOption(const char *arg)
{
    // check for the initial dash
    if (arg[0] != '-')
    {
        return 0; // not valid
    }

    // flags to check presence of characters
    int hasDorN = 0;
    int hasR = 0;

    // check the rest of the string
    for (int i = 1; arg[i] != '\0'; i++)
    {
        char c = arg[i];
        if (c == 'd' || c == 'D' || c == 'n' || c == 'N')
        {
            if (hasDorN)
                return 0; // second d or n
            hasDorN = 1;
        }
        else if (c == 'r' || c == 'R')
        {
            if (hasR)
                return 0; // second r
            hasR = 1;
        }
        else
        {
            return 0; // invalid character found
        }
    }

    // return if either required part is present
    return hasDorN || hasR;
}

void setOptions(const char *option, LogicManager *LogicManager)
{
    for (int i = 1; option[i] != '\0'; i++)
    {
        switch (option[i])
        {
        case 'd':
        case 'D':
            LogicManager->sortType = DictionarySort;
            break;
        case 'n':
        case 'N':
            LogicManager->sortType = NumericalSort;
            break;
        case 'r':
        case 'R':
            LogicManager->orderType = ReversedOrder;
            break;
        default:
            fprintf(stderr, "Invalid command snuck through error check. Got %s\n", option);
            exit(96);
        }
    }
}

void parseArgs(int argc, char **argv, LogicManager *LogicManager)
{
    // cIncludes -{d/d, n/N}{r/R} <directory/file.h>
    // 3 is max, but no min args. default will be -d <cwd>
    if (argc > 3)
    {
        fprintf(stderr, "Too many arguments, maximum of cIncludes + 2 args (-{d/d, n/N}{r/R} <directory/file.h>). Got %d args.\n", argc - 1);
        exit(95);
    }
    else if (argc == 3) // mode and path
    {
        if (isValidOption(argv[1]))
        {
            setOptions(argv[1], LogicManager);
            if (!isDirOrHeader(argv[2]))
            {
                fprintf(stderr, "If given 2 args, second arg must be path to header file or dir. Got %s\n", argv[2]);
                exit(97);
            }
            setStart(argv[2], LogicManager);
        }
        else
        {
            fprintf(stderr, "Invalid mode argument. Required format: -{d/d, n/N}{r/R}. Got %s\n", argv[1]);
            exit(96);
        }
    }
    else if (argc == 2) // just path or just mode
    {
        if (isValidOption(argv[1]))
        {
            setOptions(argv[1], LogicManager);
            LogicManager->defaultCWD = yesCWD;
        }
        else if (isDirOrHeader(argv[1]))
        {
            setStart(argv[1], LogicManager);
        }
        else
        {
            fprintf(stderr, "If given 1 arg, first arg must be path to header-file/dir or the sort/order mode. Got %s\n", argv[1]);
            exit(98);
        }
    }
    else // default == -d cwd
    {
        LogicManager->sortType = DictionarySort;
        LogicManager->orderType = NormalOrder;
        LogicManager->startPlace = startDir;
        LogicManager->defaultCWD = yesCWD;
    }
}

void findIncludesInFile(char *filePath, FileIncludes **results, int *resultCount)
{
    results[*resultCount] = (FileIncludes *)Malloc(sizeof(FileIncludes)); // space for struct
    results[*resultCount]->filePath = Strdup(filePath);                   // dynamic copy of filepath
    results[*resultCount]->numIncludes = 0;                               // initialize numIncludes

    FILE *file = Fopen(filePath, "r");
    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != -1) // read each line of file
    {
        if (strncmp(line, "#include", 8) == 0) // check for include statement
        {
            results[*resultCount]->numIncludes++; // increment score if found
        }
    }
    (*resultCount)++; // increment total results
    free(line);       // free line used in getline
    Fclose(file);
}

void findIncludesInDir(char *dirPath, FileIncludes **results, int *resultCount)
{
    DIR *dir = Opendir(dirPath);
    struct dirent *entry;
    struct stat entryStat;

    while ((entry = Readdir(dir)) != NULL)
    {
        // skip current directory or parent directory or hidden directory
        if (entry->d_name[0] == '.' || !strcmp(entry->d_name, ".."))
        {
            continue;
        }

        // allocate dirPath + entry_name + 2 (for '/' and '\0')
        char *longPath = Malloc(strlen(dirPath) + strlen(entry->d_name) + 2);
        sprintf(longPath, "%s/%s", dirPath, entry->d_name); // save path to string
        Stat(longPath, &entryStat);                         // get status
        if (!isDirOrHeader(longPath))                       // skip entries that arent header file or dir
        {
            free(longPath);
            continue;
        }

        // recursive call into subdirectory or process file
        if (S_ISDIR(entryStat.st_mode))
            findIncludesInDir(longPath, results, resultCount);
        else if (S_ISREG(entryStat.st_mode))
            findIncludesInFile(longPath, results, resultCount);
        free(longPath); // free memory used in temp file/dir path
    }
    Closedir(dir);
}

int compareFilePaths(const void *a, const void *b)
{
    const FileIncludes *fa = *(const FileIncludes **)a;
    const FileIncludes *fb = *(const FileIncludes **)b;
    return strcmp(fa->filePath, fb->filePath);
}

int compareIncludeCount(const void *a, const void *b)
{
    const FileIncludes *fa = *(const FileIncludes **)a;
    const FileIncludes *fb = *(const FileIncludes **)b;
    return fb->numIncludes - fa->numIncludes;
}

void sortResult(FileIncludes **results, LogicManager *LogicManager, int resultCount)
{
    switch (LogicManager->sortType)
    {
    case DictionarySort:
        qsort(results, resultCount, sizeof(FileIncludes *), compareFilePaths);
        break;
    case NumericalSort:
        qsort(results, resultCount, sizeof(FileIncludes *), compareIncludeCount);
        break;
    }
}

void printResult(FileIncludes **results, LogicManager *LogicManager, int resultCount)
{
    switch (LogicManager->orderType)
    {
    case NormalOrder:
        for (int i = 0; i < resultCount; i++)
        {
            fprintf(stdout, "%s:%d\n", results[i]->filePath, results[i]->numIncludes);
        }
        break;
    case ReversedOrder:
        for (int i = resultCount - 1; i >= 0; i--)
        {
            fprintf(stdout, "%s:%d\n", results[i]->filePath, results[i]->numIncludes);
        }
        break;
    }
}

void freeResult(FileIncludes **results, int resultCount)
{
    for (int i = 0; i < resultCount; i++) // for each result (FileIncludes struct)
    {
        free(results[i]->filePath); // free the path
        free(results[i]);           // free the struct
    }
    free(results); // free the results array
}
