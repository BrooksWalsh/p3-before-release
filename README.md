# p3-before-release
Trying to get started on p3 before the official release.

## The goal of p3 is to implement something similar to grep.
- Program cIncludes -{d/d, n/N}{r/R} <directory/file.h>
- The passed file/directory in argv[2] is searched for "#include" statements
- If its a directory, search for all other directories and keep track of files that are in the current directory
- If it is a file, parse through all lines and fine all times where the line begins with "#include"

## Recursive Logic
1. Enter a directory
2. If directory contains a subdirectory, enter it
    - Back to step one
3. Once there are no additional directories in the current directory
    - Then check all files in current directory
    - Collect "#include" stats (number of includes and file path)
    - Mark this directory as completed (or handle by recursive logic)
    - Exit subdirectory and move back to step 2
4. Once all subdirectories & OG directory are checked, accumulate stats, sort, and display them
