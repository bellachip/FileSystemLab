CC=gcc
CFLAGS="-Wall"
debug:clean
	$(CC) $(CFLAGS) -g -o test filesystems.h filesystems.c func.c builtins.c test.c
stable:clean
	$(CC) $(CFLAGS) -o test filesystems.h filesystems.c func.c builtins.c test.c
clean:
	rm -vfr *~ test

