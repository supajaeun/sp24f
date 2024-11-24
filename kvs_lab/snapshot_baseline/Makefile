CC = gcc
CFLAGS = -Wall -g

all: kvs

kvs: main.o do_snapshot.o do_recovery.o open.o put.o get.o close.o
	$(CC) $(CFLAGS) -o kvs main.o do_snapshot.o do_recovery.o open.o put.o get.o close.o

main.o: main.c kvs.h do_snapshot.h do_recovery.h
	$(CC) $(CFLAGS) -c main.c

do_snapshot.o: do_snapshot.c kvs.h
	$(CC) $(CFLAGS) -c do_snapshot.c

do_recovery.o: do_recovery.c kvs.h
	$(CC) $(CFLAGS) -c do_recovery.c

open.o: open.c kvs.h
	$(CC) $(CFLAGS) -c open.c

put.o: put.c kvs.h
	$(CC) $(CFLAGS) -c put.c

get.o: get.c kvs.h
	$(CC) $(CFLAGS) -c get.c

close.o: close.c kvs.h
	$(CC) $(CFLAGS) -c close.c

clean:
	rm -f *.o kvs
