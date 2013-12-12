CC = gcc  -Wno-deprecated -Wall
OBJ = a2
%.o: %.cpp
	$(CC) -o $@ $<

all: a2

a2: a2.cpp
	$(CC) -o a2 a2.cpp -lstdc++
clean:
	rm -rf $(OBJ)	
