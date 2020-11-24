#pragma once
#include <cstddef>
#include <new>

template <class T>
struct Allocator
{
    Allocator() = default;
    T* allocate(size_t n) {
        if (n == 0) return nullptr;
        return static_cast<T*>(new T[n]);
    }
    void deallocate(T* p) noexcept {
        delete[] p;
    }
};
