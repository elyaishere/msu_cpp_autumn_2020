#pragma once
#include <exception>

template <class T>
class iterator {
    public:
    iterator(T* ptr, bool r): iter(ptr), rev(r) {}

    T& operator*() const {
        return *iter;
    }
    bool operator==(const iterator& other) const {
        return iter == other.iter;
    }
    bool operator!=(const iterator& other) const {
        return iter != other.iter;
    }
    bool operator<(const iterator& other) const {
        return (rev) ? other.iter < iter : iter < other.iter;
    }
    bool operator<=(const iterator& other) const {
        return (rev) ? other.iter <= iter : iter <= other.iter;
    }
    bool operator>(const iterator& other) const {
        return (rev) ? other.iter > iter : iter > other.iter;
    }
    bool operator>=(const iterator& other) const {
        return (rev) ? other.iter >= iter : iter >= other.iter;
    }
    iterator& operator++() {
        if (rev) {
            iter--;
            return *this;
        }
        iter++;
        return *this;
    }
    iterator operator++(int) {
        if (rev) {
            auto t_iter = iter;
            --iter;
            return iterator(t_iter, rev);
        }
        auto t_iter = iter;
        ++iter;
        return iterator(t_iter);
    }
    iterator& operator--() {
        if (rev) {
            iter++;
            return *this;
        }
        iter--;
        return *this;
    }
    iterator operator+(ptrdiff_t shf) const {
        if (rev) {
            return iterator(iter - shf);
        }
        return iterator(iter + shf);
    }
    iterator operator-(ptrdiff_t shf) const {
        if (rev) {
            return iterator(iter + shf);
        }
        return iterator(iter - shf);
    }
    iterator& operator+=(ptrdiff_t shf) {
        if (rev) {
            iter -= shf;
            return *this;
        }
        iter += shf;
        return *this;
    }
    iterator& operator-=(ptrdiff_t shf) {
        if (rev) {
            iter += shf;
            return *this;
        }
        iter -= shf;
        return *this;
    }
    ptrdiff_t operator-(const iterator<T>& other) const {
        if (rev) {
            if (other.iter < iter)
                throw std::invalid_argument("Wrong argument");
            return other.iter - iter;
        }
        if (iter < other.iter)
            throw std::invalid_argument("Wrong argument");
        return iter - other.iter;
    }

    private:
    T* iter;
    bool rev;
};
