#!/bin/bash

unalias -a    # remove any aliases user may have in their environment

function echoErr { printf "%s\n" "$*" 1>&2; }   # function to simplify printing error messages to stderr

function printHelp {
  echoErr "Usage:   cIncludes.sh [-d|-o|-do|-dr|-on|-or|-n|-nr|-dor|-nor] [.h file|directory]"
  echoErr "Where:   -d = search file or recursively search directory for number of includes in .h file(s); display results in dictionary order"
  echoErr "         -o = only search current directory; do not descind into subdirectories"
  echoErr "         -r = display results in reverse order"
  echoErr "         -n = display results in numerical order"
  echoErr "Default: -d"
  echoErr "Notes:   all options are case-insensitive and must be grouped together; order does not matter"
}

function findIncludesInFile { 
  file=$1
  numIncludes=$(grep -c ^#include "$file" 2>/dev/null)
  re='^[0-9]+$'  # regular expression to check for variable being an integer
  if [[ "$numIncludes" =~ $re ]] ; then
    echo "$(pwd)/$file:$numIncludes"
  else
    echoErr "Issue searching $file for '#include'"; exit 98
  fi
}

function findIncludesInDir {  # function to perform the appropriate call to recursively search a directory
  options=$1
  dir=$2
  case "$options" in
    d)                             grep -rc ^#include $dir     | grep "\.h:" | sort -d         ;;
    o|'do'|od)                     grep -c  ^#include $dir/*.h               | sort -d         ;;
    dr|rd)                         grep -rc ^#include $dir     | grep "\.h:" | sort -d -r      ;;
    ro|or|odr|ord|dor|rod|dro|rdo) grep -c  ^#include $dir/*.h               | sort -d -r      ;;
    n)                             grep -rc ^#include $dir     | grep "\.h:" | sort -t: -k2    ;;
    no|on)                         grep -c  ^#include $dir/*.h               | sort -t: -k2    ;;
    nr|rn)                         grep -rc ^#include $dir     | grep "\.h:" | sort -t: -k2 -r ;;
    onr|orn|nor|ron|nro|rno)       grep -c  ^#include $dir/*.h               | sort -t: -k2 -r ;;
    *)                             echoErr "Invalid option(s) '$options'"; exit 99             ;;
  esac
}

case "$#" in  # check how many arguments were passed
  0) # default is to recursively search for .h files in the current directory and any subdirectories
     options="d"
     dir="$(pwd)"
     findIncludesInDir "$options" "$dir"
     ;;
  1) # check if argument passed is an option (starts with '-') or a file or a directory
     if [[ "$1" == -* ]]; then    # options passed, default to recursively search for .h files
        options=$(echo "$1" | tr '[:upper:]' '[:lower:]' | sed "s/-//")
       if [[ "$1" =~ h ]]; then
         printHelp
         exit 0
       else
         dir="$(pwd)"
         findIncludesInDir "$options" "$dir"
       fi
     else                         
       if [ -d "$1" ]; then           # directory passed
         options="d"
         if [[ "$1" == . ]]; then dir="$(pwd)"; else dir="$1"; fi
         findIncludesInDir "$options" "$dir"
       elif [[ "$1" == *.h ]]; then   # file passed
         if [ -f "$1" ]; then
           findIncludesInFile $1
         else
           echoErr "'$1' is not a header file or does not exist"; exit 97
         fi
       else
         echoErr "Invalid argument '$1', expecting a .h file or an existing directory"; exit 96
       fi
     fi
     ;;
  2) # check if first argument is an option (starts with '-') and second argument is a file or a directory
     if [[ "$1" == -* ]]; then    # options passed, default to recursively search for .h files
       options=$(echo "$1" | tr '[:upper:]' '[:lower:]' | sed "s/-//")
       if [ -d "$2" ]; then           # directory passed
         if [[ "$2" == . ]]; then dir="$(pwd)"; else dir="$2"; fi
         findIncludesInDir "$options" "$dir"
       elif [[ "$2" == *.h ]]; then   # file passed
         if [ -f "$2" ]; then
           findIncludesInFile $2
         else
           echoErr "'$2' is not a header file or does not exist"; exit 97
         fi
       else
         echoErr "Invalid argument '$2', expecting a .h file or an existing directory"; exit 96
       fi
     else
       echoErr "Invalid first argument '$1', expecting -(d|D|n|N|r|R)"; exit 95
     fi
     ;;
  *) # invalid
     echoErr "Invalid number of arguments"; printHelp; exit 94
     ;;
esac
