CC=g++
FLAGS=--std=c++17
all: test_alloc
test_alloc: main.o Allocator.o
	$(CC) $(FLAGS) -o test_alloc main.o Allocator.o
main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp
Allocator.o: Allocator.cpp Allocator.h
	$(CC) $(FLAGS) -c Allocator.cpp
clean:
	rm -rf *.o test_alloc
test: 
	valgrind ./test_alloc
