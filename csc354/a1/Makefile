CC = csim
OBJ = a1a a1b a1c
%.o: %.c
	$(CC) -o $@ $<

all: part1 part2 part3

part1: a1a.c
	$(CC) -o a1a a1a.c
part2: a1b.c
	$(CC) -o a1b a1b.c
part3: a1c.c
	$(CC) -o a1c a1c.c
clean:
	rm -rf $(OBJ)	
