#pragma once
#include <cstdio>

using namespace std;

class Allocator
{
public:
    explicit Allocator (size_t size);
    void makeAllocator (size_t maxSize);
    char* alloc(size_t size);
    void reset();
    ~Allocator();

private:
    size_t _size = 0;
    char * _offset = nullptr;
    char * _head = nullptr;
};