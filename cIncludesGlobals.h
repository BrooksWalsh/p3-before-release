#ifndef CINCLUDESGLOBALS_H
#define CINCLUDESGLOBALS_H

typedef enum
{
    DictionarySort = 0,
    NumericalSort = 1
} SortType;

typedef enum
{
    NormalOrder = 0,
    ReversedOrder = 1
} OrderType;

typedef struct
{
    SortType sortType;
    OrderType orderType;
} LogicManager;

typedef struct
{
    const char *filePath;
    int numIncludes;
} FileIncludes;

#endif