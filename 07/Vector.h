#ifndef INC_07_VECTOR_H
#define INC_07_VECTOR_H
#include <exception>
#include <limits>
#include <iterator>


template <class T>
class Allocator {
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;

public:
    pointer allocate(size_type count) {
        pointer p = static_cast<T*> (::operator new(sizeof(value_type) * count));
        if (p == nullptr) {
            throw std::bad_alloc();
        }
        return p;
    }

    void deallocate(pointer p, size_type count) {
        ::operator delete(p);
    }

    size_t max_size() const noexcept {
        return std::numeric_limits<size_t>::max() / sizeof(value_type);
    }

    void construct(pointer p, const_reference val, size_type n) {
        for (size_type i = 0; i < n; i++) {
            new (p + i) T(val);
        }
    }

    void construct(pointer p, size_type n) {
        for (size_type i = 0; i < n; i++) {
            new (p + i) T();
        }
    }

    void destroy(pointer p, size_type n) {
        for (size_type i = 0; i < n; i++) {
            (p[i]).~T();
        }
    }
};

template <class T>
class Iterator {
    using size_type = std::size_t;
    using reference = T&;
public:
    explicit Iterator(T* ptr, bool reverse) : ptr_(ptr), reverse(reverse) {}

    bool operator==(const Iterator<T>& other) const {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const Iterator<T>& other) const {
        return !(*this == other);
    }

    reference operator*() const {
        return *ptr_;
    }

    Iterator& operator++() {
        if (reverse) {
            --ptr_;
        } else {
            ++ptr_;
        }
        return *this;
    }

    Iterator& operator+(size_type n) {
        if (reverse) {
            ptr_ -= n;
        } else {
            ptr_ += n;
        }
        return *this;
    }

    Iterator& operator-(size_type n) {
        if (reverse) {
            ptr_ += n;
        } else {
            ptr_ -= n;
        }
        return *this;
    }

    Iterator operator++(int) {
        Iterator it(ptr_);
        if (reverse) {
            ptr_--;
        } else {
            ptr_++;
        }
        return it;
    }

    Iterator& operator--() {
        if (reverse) {
            ptr_++;
        } else {
            ptr_--;
        }
        return *this;
    }

    Iterator operator--(int) {
        Iterator it(ptr_);
        if (reverse) {
            ptr_++;
        } else {
            ptr_--;
        }
        return it;
    }

private:
    T* ptr_;
    bool reverse;
};

template <class T, class Alloc = Allocator<T>>
class Vector {
public:
    using iterator = Iterator<T>;
    using value_type = T;
    using size_type = std::size_t;
    using const_reference = const T&;
    using pointer = T *;
    using reference = T&;

    explicit Vector() {
        alloc_ = Alloc();
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    explicit Vector(size_type count) {
        alloc_ = Alloc();
        data_ = alloc_.allocate(count);
        alloc_.construct(data_, count);
        size_ = count;
        capacity_ = count;
    }

    Vector(size_type count, const_reference defaultValue) {
        alloc_ = Alloc();
        data_ = alloc_.allocate(count);
        alloc_.construct(data_, defaultValue, count);
        size_ = count;
        capacity_ = count;
    }

    ~Vector() {
        alloc_.destroy(data_, size_);
        alloc_.deallocate(data_, capacity_);
    }

    iterator begin() noexcept {
        return iterator(data_, false);
    }
    iterator rbegin() noexcept {
        return iterator(data_ + size_ - 1, true);
    }

    iterator end() noexcept {
        return iterator(data_ + size_, false);
    }
    iterator rend() noexcept {
        return iterator(data_- 1, true);
    }

    reference operator[](size_type ind) {
        return data_[ind];
    }

    const_reference operator[](size_type ind) const {
        return data_[ind];
    }

    void push_back(value_type&& value) {
        if (capacity_ == 0) {
            data_ = alloc_.allocate(1);
            new (data_) T(value);
            size_ = 1;
            capacity_++;
        } else if (size_ == capacity_) {
            pointer new_data = alloc_.allocate(capacity_ * 2);
            std::copy(data_, data_ + size_, new_data);
            alloc_.destroy(data_, size_);
            alloc_.deallocate(data_, capacity_);
            data_ = new_data;
            data_[size_++] = std::move(value);
            capacity_ *= 2;
        } else if (size_ < capacity_) {
            data_[size_++] = std::move(value);
        }
    }

    void push_back(const_reference value) {
        if (capacity_ == 0) {
            data_ = alloc_.allocate(1);
            new (data_) T(value);
            size_ = 1;
            capacity_++;
        } else if (size_ == capacity_) {
            pointer new_data = alloc_.allocate(capacity_ * 2);
            std::copy(data_, data_ + size_, new_data);
            alloc_.destroy(data_, size_);
            alloc_.deallocate(data_, capacity_);
            data_ = new_data;
            new (data_ + size_) T(value);
            size_++;
            capacity_ *= 2;
        } else if (size_ < capacity_) {
            new (data_ + size_) T(value);
            size_++;
        }
    }

    void pop_back() {
        alloc_.destroy(data_ + size_ - 1, 1);
        size_--;
    }

    void reserve(size_type count) {
        if (count > alloc_.max_size()) {
            throw std::length_error("an attempt to reserve too much for vector");
        }
        if (count > capacity_) {
            pointer new_data = alloc_.allocate(count);
            std::copy(data_, data_ + size_, new_data);
            alloc_.destroy(data_, size_);
            alloc_.deallocate(data_, capacity_);
            data_ = new_data;
            capacity_ = count;
        }
    }

    void resize(size_type newSize) {
        if (newSize < size_) {
            alloc_.destroy(data_ + newSize, size_ - newSize);
            size_ = newSize;
        } else if (newSize > size_) {
            if (newSize > capacity_) {
                pointer new_data = alloc_.allocate(newSize);
                std::copy(data_, data_ + size_, new_data);
                alloc_.destroy(data_, size_);
                alloc_.deallocate(data_, capacity_);
                data_ = new_data;
                capacity_ = newSize;
            }
            alloc_.construct(data_ + size_, newSize - size_);
            size_ = newSize;
        }
    }

    void resize(size_type newSize, const_reference defaultValue) {
        if (newSize < size_) {
            alloc_.destroy(data_ + newSize, size_ - newSize);
            size_ = newSize;
        } else if (newSize > size_) {
            if (newSize > capacity_) {
                pointer new_data = alloc_.allocate(newSize);
                std::copy(data_, data_ + size_, new_data);
                alloc_.destroy(data_, size_);
                alloc_.deallocate(data_, capacity_);
                data_ = new_data;
                capacity_ = newSize;
            }
            alloc_.construct(data_ + size_, defaultValue, newSize - size_);
            size_ = newSize;
        }
    }

    bool empty() {
        return size_ == 0;
    }

    void clear() noexcept {
        alloc_.destroy(data_, size_);
        size_ = 0;
    }

    size_type size() {
        return size_;
    }

    size_type capacity() {
        return capacity_;
    }

private:
    Alloc alloc_;
    size_type size_;
    size_type capacity_;
    pointer data_;
};

#endif //INC_07_VECTOR_H
