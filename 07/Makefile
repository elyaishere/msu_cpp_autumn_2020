CC=g++
FLAGS=--std=c++17
all: test_vector
test_vector: main.o
	$(CC) $(FLAGS) -o test_vector main.o
main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp
clean:
	rm -rf *.o test_vector
test:
	valgrind -s ./test_vector