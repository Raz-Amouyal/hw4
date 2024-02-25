CC=gcc
CFLAGS=-Wall -g -std=c99 -c -fpic
CCLINK=$(CC)
OBJS=grades.o
EXEC=e
RM=rm -f *.o *grades.so 

libgrades.so: $(OBJS)
	$(CCLINK) -shared $(OBJS) -o libgrades.so -llinked-list -L.

grades.o: grades.c grades.h
	$(CC) $(CFLAGS) grades.c

clean:
	$(RM) 
