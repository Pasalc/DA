CC=g++
CFLAGS= -pedantic -pg -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c 

all: main.o Node.o NodeArray.o  
	$(CC) -pg main.o Node.o NodeArray.o -o main
	make clean
NodeArray.o: NodeArray.h 
	$(CC) $(CFLAGS) NodeArray.cpp	
Node.o: Node.h RadixSort.h 
	$(CC) $(CFLAGS) Node.cpp
main.o: Variables.h
	$(CC) $(CFLAGS) main.cpp
clean:
	rm -rf *.o