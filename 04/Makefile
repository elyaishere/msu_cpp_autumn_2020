CC=g++
FLAGS=--std=c++17
all: test_bigint
test_bigint: main.o BigInt.o
	$(CC) $(FLAGS) -o test_bigint main.o BigInt.o
main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp
BigInt.o: BigInt.cpp BigInt.h
	$(CC) $(FLAGS) -c BigInt.cpp
clean:
	rm -rf *.o test_bigint
test:
	valgrind -s ./test_bigint