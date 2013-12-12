CC = gcc
CFLAGS = -O -lpthread

all: measure 

measure: measure.c etime.h etime.o
	$(CC) $(CFLAGS) -o measure measure.c etime.o

example: example.c etime.h etime.o
	$(CC) $(CFLAGS) -o example example.c etime.o

etime.o: etime.c etime.h
	$(CC) $(CFLAGS) -c etime.c

clean:
	rm -f *.o example *~

