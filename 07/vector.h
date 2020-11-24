#pragma once
#include "Allocator.h"
#include <exception>
#include "iterator.h"

template <class T, class Alloc = Allocator<T>>
class vector {
    public:

    // member functions
    explicit vector(size_t n = 0): _size(n), _capacity(n) {
        data = alloc.allocate(n);
    }
    ~vector() {
        alloc.deallocate(data);
    }
    vector(const vector& v): _size(v._size), _capacity(v._size) {
        data = alloc.allocate(_capacity);
        for (size_t i = 0; i < _size; ++i)
            data[i] = v.data[i];
    }
    vector(vector&& v): _size(v._size), _capacity(v._capacity) {
        data = v.data;
        v.data = nullptr;
        v._size = 0;
        v._capacity = 0;
    }
    vector& operator=(const vector& v) {
        if (data == v.data) return *this;
        alloc.deallocate(data);
        _size = v._size;
        _capacity = v._size;
        data = alloc.allocate(_size);
        for (size_t i = 0; i < _size; ++i)
            data[i] = v.data[i];
        return *this;
    }
    vector& operator=(vector&& v) {
        if (data == v.data) return *this;
        alloc.deallocate(data);
        _size = v._size;
        _capacity = v._capacity;
        data = v.data;
        v.data = nullptr;
        v._size = 0;
        v._capacity = 0;
        return *this;
    }
    //--------------------------
    // iterators
    iterator<T> begin() noexcept {
        return iterator(data, false);
    }
    iterator<T> end() noexcept {
        return iterator(data + _size, false);
    }
    iterator<T> rbegin() noexcept {
        return iterator(data + _size - 1, true);
    }
    iterator<T> rend() noexcept {
        return iterator(data - 1, true);
    }
    //--------------------------
    // element access
    T& operator[](size_t i) {
        if (i >= _size || i < 0)
            throw std::out_of_range("Wrong index");
        return data[i];
    }
    const T& operator[](size_t i) const {
        if (i >= _size || i < 0)
            throw std::out_of_range("Wrong index");
        return data[i];
    }
    //--------------------------
    // capacity
    bool empty() const noexcept {
        return _size == 0;
    }
    size_t capacity() const noexcept {
        return _capacity;
    }
    void reserve(size_t new_capacity) {
        if (_capacity < new_capacity) {
            T* new_data = alloc.allocate(new_capacity);
            for (size_t i = 0; i < size; ++i) 
                new_data[i] = std::move(data[i]);
            if (data != nullptr)
                alloc.deallocate(data);
            data = new_data;
            _capacity = new_capacity;
        }
    }
    size_t size() const noexcept {
        return _size;
    }
    //--------------------------
    // modifiers
    void resize(size_t new_size, const T & value = T()) {
        if (new_size >= _capacity)
            reserve(2 * _capacity + 1);
        for (size_t i = _size; i < new_size; ++i)
            data[i] = value;
        _size = new_size;
    }
    void push_back(const T& value) {
        if (_size == _capacity) {
            reserve(2 * _capacity + 1);
        }
        data[_size++] = value;
    }
    void push_back(T&& value) {
        if (_size == _capacity) {
            reserve(2 * _capacity + 1);
        }
        data[_size++] = std::move(value);
    }
    void pop_back() {
        if (empty()) 
            throw std::out_of_range("Vector is empty");
        _size -= 1;
    }
    template <class... Args>
    void emplace_back(Args&&... args) {
        if (_size == _capacity)
            reserve(2 * _capacity + 1);
        new (static_cast<void*>(data + _size)) T(std::forward<Args>(args)...);
        ++_size;
    }
    void clear() noexcept {
        for (size_t i = 0; i < _size; ++ i)
            (data + i)->~T(); 
        _size = 0;
    }

    private:
    size_t _size;
    size_t _capacity;
    T* data;
    Alloc alloc;
};
