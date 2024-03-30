CFLAGS=-g -c -Wall -Wextra -Wwrite-strings
LFLAGS=-g

cIncludes: cIncludes.o cIncludesFuncs.o syscalls.o
	gcc ${LFLAGS} -o cIncludes cIncludes.o cIncludesFuncs.o syscalls.o
cIncludes.o: cIncludes.c cIncludesFuncs.h cIncludesGobals.h
	gcc ${CFLAGS} cIncludes.c
cIncludesFuncs.o: cIncludesFuncs.c syscalls.h cIncludesGobals.h
	gcc ${CFLAGS} cIncludesFuncs.c
syscalls.o: syscalls.c
	gcc ${CFLAGS} syscalls.c
clean:
	rm -f *.o
	rm -f cIncludes