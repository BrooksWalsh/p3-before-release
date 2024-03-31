#include "cIncludesGlobals.h"
#include "cIncludesFuncs.h"

int main(int argc, char **argv)
{
    // initialize array for holding FileIncludes structs for each file searched
    FileIncludes *results[128];

    // initialize logic manager (two fields: sort type, order mode)
    LogicManager LogicManager = {DictionarySort, ReversedOrder};

    // parseargs returns truthy if directory and falsy if file
    if (parseArgs(argc, argv, &LogicManager)) // directory (or cwd when nothing passed)
    {
        findIncludesInDir(argv[2], results); // safe b/c parseArgs checks argv[2]
        sortResult(results, &LogicManager);
        printResult(results);
    }
    else // header file passed directly
    {
        findIncludesInFile(argv[2], results); // safe b/c parseArgs checks argv[2]
        printResult(results);
    }
    return 0;
}