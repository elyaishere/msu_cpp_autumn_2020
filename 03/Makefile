CC=g++
FLAGS=--std=c++17
all: test_matrix
test_matrix: main.o Matrix.o
	$(CC) $(FLAGS) -o test_matrix main.o Matrix.o
main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp
Matrix.o: Matrix.cpp Matrix.h
	$(CC) $(FLAGS) -c Matrix.cpp
clean:
	rm -rf *.o test_matrix
test:
	valgrind -s ./test_matrix
