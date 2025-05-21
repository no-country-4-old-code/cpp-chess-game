#pragma once
#include <array>
#include <iterator>

template <typename TYPE, size_t SIZE>
class StackVector {
        std::array<TYPE, SIZE> _array;
        unsigned int _size{0};

    public:
        StackVector() = default;
        size_t size() const { return _size; }

        void push(TYPE obj) {
            if (_size < _array.size()) {
                _array[_size] = obj;
                ++_size;
            }
        }

        // Iterator (mutable)
        class Iterator {
                TYPE* _ptr;

            public:
                explicit Iterator(TYPE* ptr) : _ptr(ptr) {}
                TYPE& operator*() const { return *_ptr; }
                Iterator& operator++() {
                    ++_ptr;
                    return *this;
                }
                bool operator!=(const Iterator& other) const { return _ptr != other._ptr; }
        };

        // ConstIterator
        class ConstIterator {
                const TYPE* _ptr;

            public:
                explicit ConstIterator(const TYPE* ptr) : _ptr(ptr) {}
                const TYPE& operator*() const { return *_ptr; }
                ConstIterator& operator++() {
                    ++_ptr;
                    return *this;
                }
                bool operator!=(const ConstIterator& other) const { return _ptr != other._ptr; }
        };

        Iterator begin() { return Iterator(_array.data()); }
        Iterator end() { return Iterator(_array.data() + _size); }

        ConstIterator begin() const { return ConstIterator(_array.data()); }
        ConstIterator end() const { return ConstIterator(_array.data() + _size); }

        TYPE& operator[](size_t idx) {
            assert(idx < SIZE);
            return _array[idx];
        }
        const TYPE& operator[](size_t idx) const {
            assert(idx < SIZE);
            return _array[idx];
        }
};