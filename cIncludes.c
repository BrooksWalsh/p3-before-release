#include <stdlib.h> // for free
#include "cIncludesGlobals.h"
#include "cIncludesFuncs.h"
#include "syscalls.h"

int main(int argc, char **argv)
{
    // TODO: check for full capacity and realloc (very unlikely but gotta check)
    // initialize results array and logic manager
    int capacity = 128;
    int resultCount = 0;
    FileIncludes **results = Malloc(capacity * sizeof(FileIncludes *));
    LogicManager LogicManager = {DictionarySort, NormalOrder, startDir, noCWD};

    // parse args and update logic manager accordingly
    parseArgs(argc, argv, &LogicManager);
    if (LogicManager.startPlace) // directory (or cwd when nothing passed)
    {
        if (LogicManager.defaultCWD)
        {
            char *cwd = Getcwd();
            findIncludesInDir(cwd, results, &resultCount);
            free(cwd);
        }
        else // given directory to search
        {
            findIncludesInDir(argv[(argc == 2) ? 1 : 2], results, &resultCount);
        }
        sortResult(results, &LogicManager, resultCount);
    }
    else // header file passed directly
    {
        findIncludesInFile(argv[(argc == 2) ? 1 : 2], results, &resultCount);
    }
    printResult(results, &LogicManager, resultCount);
    freeResult(results, resultCount);
    return 0;
}