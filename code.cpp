#include <algorithm>
#include <cstddef>
#include <iostream>

template <typename T>
class Vector{
private:
    size_t cp, sz;
    T* start;

public:
    Vector() {
        cp = 0;
        sz = 0;
        start = nullptr;
    }

    Vector(size_t szValue) {
        cp = szValue;
        sz = szValue;
        start = static_cast<T*>(operator new(sizeof(T) * cp));
        for (size_t i = 0; i != sz; ++i) {
            new (start + i) T();
        }
    }

    void clear() {
        for (size_t i = 0; i != sz; ++i) {
            start[i].~T();
        }
        operator delete(start);
        cp = 0;
        sz = 0;
        start = nullptr;
    }

    T& operator[] (int idx) const {
        return *(start + idx);
    }

    Vector& operator =(const Vector& v) {
        T* newStart = static_cast<T*>(operator new(sizeof(T) * v.cp));
        size_t i;
        for (i = 0; i != v.sz; ++i) {
            try {
                new (newStart + i) T(v[i]);  // NOLINT
            } catch (...) {
                for (size_t j = 0; j != i; ++j) {
                    newStart[i].~T();
                }
                operator delete(newStart);
                return (*this);
            }
        }
        clear();
        sz = v.sz;
        cp = v.cp;
        start = newStart;
        return (*this);
    }

    Vector(const Vector& v) {
        (*this) = v;
    }

    void Realloc(const size_t& newCapacity) {
        T* newStart = static_cast<T*>(operator new(sizeof(T) * newCapacity));
        size_t i;
        for (i = 0; i != sz; ++i) {
            try {
                new (newStart + i) T(*(start + i));  // NOLINT
            } catch (...) {
                for (size_t j = 0; j != i; ++j) {
                    newStart[j].~T();
                }
                operator delete(newStart);
            }
        }
        size_t oldSz = sz;
        clear();
        sz = oldSz;
        cp = newCapacity;
        start = newStart;
    }

    void push_back(const T& value) {
        if (cp == sz) {
            if (sz == 0) {
                Realloc(1);
            } else {
                Realloc(2 * cp);
            }
        }
        if (cp == sz) {
            return;
        }
        ++sz;
        try {
            new(start + sz - 1) T(value);  // NOLINT
        } catch (...) {}
    }

    void push_back(T&& rvalue) {
        if (cp == sz) {
            if (sz == 0) {
                Realloc(1);
            } else {
                Realloc(2 * cp);
            }
        }
        if (cp == sz) {
            return;
        }
        ++sz;
        try {
            new(start + sz - 1) T(std::move(rvalue));  // NOLINT
        } catch (...) {}
    }

    void pop_back() {
        start[sz-1].~T();
        --sz;
    }

    size_t size() const {
        return sz;
    }

    size_t capacity() const {
        return cp;
    }

    void reserve(const size_t& new_cap) {
        if (cp >= new_cap) {
            return;
        }
        Realloc(new_cap);
    }

    void resize(const size_t& newSz) {
        if (newSz < sz) {
            for (size_t i = newSz; i != sz; ++i) {
                start[i].~T();
            }
        } else if (newSz >= sz && newSz <= cp) {
            size_t i;
            for (i = sz; i != newSz; ++i) {
                try {
                    new (start + i) T();
                } catch (...) {
                    for (size_t j = sz; j != i; ++j) {
                        start[j].~T();
                    }
                    return;
                }
            }
        } else {
            T* newStart = static_cast<T*>(operator new(sizeof(T) * newSz));
            size_t i;
            for (i = 0; i != sz; ++i) {
                try {
                    new (newStart + i) T(*(start + i));
                } catch (...) {
                    for (size_t j = 0; j != i; ++j) {
                        newStart[j].~T();
                    }
                    operator delete(newStart);
                    return;
                }
            }
            for (i = sz; i < newSz; ++i) {
                try {
                    new (newStart + i) T();
                } catch (...) {
                    for (size_t j = 0; j != i; ++j) {
                        newStart[j].~T();
                    }
                    operator delete(newStart);
                    return;
                }
            }
            clear();
            start = newStart;
            cp = newSz;
        }
        sz = newSz;
    }

    ~Vector() {
        for (size_t i = 0; i != sz; ++i) {
            start[i].~T();
        }
        operator delete (start);
    }

    T* begin() {
        return start;
    }
    T* end() {
        return (start + sz);
    }
    void swap(Vector& other) {
        std::swap(start, other.start);
        std::swap(cp, other.cp);
        std::swap(sz, other.sz);
    }
};
