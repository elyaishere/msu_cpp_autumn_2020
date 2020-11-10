CC=g++
FLAGS=--std=c++17
all: test_serializer
test_serializer: main.o serializer.o
	$(CC) $(FLAGS) -o test_serializer main.o serializer.o
main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp
serializer.o: serializer.cpp serializer.h
	$(CC) $(FLAGS) -c serializer.cpp
clean:
	rm -rf *.o test_serializer
test:
	valgrind -s ./test_serializer