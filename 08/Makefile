CC=g++
all: pool
pool: main.o
	$(CC) main.o -pthread -o pool --std=c++14
main.o: main.cpp
	$(CC) main.cpp -c --std=c++14
clean:
	rm -rf *.o pool
test: 
	./pool