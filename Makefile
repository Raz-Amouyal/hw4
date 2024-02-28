CC=gcc
CFLAGS=-Wall -g -std=c99 -c -fpic
CCLINK=$(CC)
OBJS=grades.o
EXEC=libgrades.so
RM=rm -f *.o *grades.so 

$(EXEC): $(OBJS)
	$(CCLINK) -shared $(OBJS) -o $(EXEC) -llinked-list -L.

$(OBJS): grades.c grades.h
	$(CC) $(CFLAGS) grades.c

clean:
	$(RM)
