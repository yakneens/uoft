CC = gcc
CFLAGS = -O

all:  bridge_1 bridge_2 bridge_3 schedsim_1 schedsim_2

1: bridge_1.c bridge_1.h
	$(CC) $(CFLAGS) -o bridge_1 bridge_1.c -lpthread
2: bridge_2.c bridge_2.h
	$(CC) $(CFLAGS) -o bridge_2 bridge_2.c -lpthread
3: bridge_3.c bridge_3.h
	$(CC) $(CFLAGS) -o bridge_3 bridge_3.c -lpthread
s1: schedsim_1.c schedsim_1.h
	$(CC) $(CFLAGS) -o schedsim_1 schedsim_1.c
s2: schedsim_2.c schedsim_2.h
	$(CC) $(CFLAGS) -o schedsim_2 schedsim_2.c

