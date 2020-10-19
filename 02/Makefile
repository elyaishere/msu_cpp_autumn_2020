CC=g++
FLAGS=--std=c++17
all: test_parser
test_parser: main.o TokenParser.o
	$(CC) $(FLAGS) -o test_parser main.o TokenParser.o
main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp
TokenParser.o: TokenParser.cpp TokenParser.h
	$(CC) $(FLAGS) -c TokenParser.cpp
clean:
	rm -rf *.o test_parser
test: 
	valgrind ./test_parser