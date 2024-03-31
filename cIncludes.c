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
    switch (LogicManager.startPlace) // starting at file or dir?
    {
    case startDir:
        switch (LogicManager.defaultCWD) // given dir or default to CWD?
        {
        case yesCWD:
            char *cwd = Getcwd();
            findIncludesInDir(cwd, results, &resultCount);
            free(cwd);
            break;
        case noCWD:
            findIncludesInDir(argv[(argc == 2) ? 1 : 2], results, &resultCount);
            break;
        }
        sortResult(results, &LogicManager, resultCount);
        break;
    case startFile:
        findIncludesInFile(argv[(argc == 2) ? 1 : 2], results, &resultCount);
        break;
    }
    printResult(results, &LogicManager, resultCount);
    freeResult(results, resultCount);
    return 0;
}