CC=g++
FLAGS=--std=c++17
all: test_format
test_format: main.o
	$(CC) $(FLAGS) -o test_format main.o
main.o: main.cpp format.h
	$(CC) $(FLAGS) -c main.cpp
clean:
	rm -rf *.o test_format
test:
	valgrind -s ./test_format