#include "Allocator.h"

Allocator::Allocator (size_t size): _size(size) {
    _head = _offset = new char[_size];
}

void Allocator::makeAllocator (size_t maxSize) {
    delete[] _head;
    _size = maxSize;
    _head = _offset = new char[_size];
}

char* Allocator::alloc (size_t size) {
    if (_offset + size <= _head + _size) {
        char * res = _offset;
        _offset += size;
        return res;
    }
    else {
        return nullptr;
    }
}

void Allocator::reset () {
    _offset = _head;
}

Allocator::~Allocator() {
    delete[] _head;
}