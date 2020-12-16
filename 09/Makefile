CC=g++
all: sorting
sorting: main.o
	$(CC) main.o -pthread -o sorting --std=c++14
main.o: main.cpp
	$(CC) main.cpp -c --std=c++14
clean:
	rm -rf *.o sorting src_*
test: 
	./sorting