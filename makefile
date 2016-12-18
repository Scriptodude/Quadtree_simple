CC = g++
OPTIONS = -Wall -O2 -std=c++14

all: main.o quadtree.hpp
	$(CC) $(OPTIONS) -o quadtree_test main.o quadtree.hpp
main.o: main.cpp quadtree.hpp
	$(CC) $(OPTIONS) -c main.cpp

clean:
	rm -rf *.o *_test*