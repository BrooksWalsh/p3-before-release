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

typedef enum
{
    startFile = 0,
    startDir = 1
} StartPlace;

typedef enum
{
    noCWD = 0,
    yesCWD = 1
} DefaultCWD;

typedef struct
{
    SortType sortType;
    OrderType orderType;
    StartPlace startPlace;
    DefaultCWD defaultCWD;
} LogicManager;

typedef struct
{
    char *filePath;
    int numIncludes;
} FileIncludes;

#endif