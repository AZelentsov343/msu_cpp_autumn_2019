#ifndef INC_07_VECTOR_H
#define INC_07_VECTOR_H
#include <exception>
#include <limits>
#include <iterator>


template <class T>
class Allocator {
public:
    std::unique_ptr<T[]> allocate(size_t count) {
        return std::make_unique<T[]>(count);
    }

    void deallocate(std::unique_ptr<T[]> ptr, size_t count) {
        for (int i = 0; i < count; i++) {
            ptr[i].~T();
        }
    }

    size_t max_size() const noexcept {
        return std::numeric_limits<size_t>::max();
    }
};

template <class T>
class Iterator {
    T* ptr_;
    bool reverse;
public:
    explicit Iterator(T* ptr, bool reverse) : ptr_(ptr), reverse(reverse) {}

    bool operator==(const Iterator<T>& other) const {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const Iterator<T>& other) const {
        return !(*this == other);
    }

    T& operator*() const {
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

    Iterator& operator+(size_t n) {
        if (reverse) {
            ptr_ -= n;
        } else {
            ptr_ += n;
        }
        return *this;
    }

    Iterator& operator-(size_t n) {
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

};

template <class T, class Alloc = Allocator<T>>
class Vector {
public:
    using iterator = Iterator<T>;
    using value_type = T;
    using const_reference = const T&;

    explicit Vector() {
        alloc_ = Alloc();
        data_ = alloc_.allocate(2);
        size_ = 0;
        capacity_ = 2;
    }

    explicit Vector(size_t count) {
        alloc_ = Alloc();
        data_ = alloc_.allocate(count);
        size_ = 0;
        capacity_ = count;
    }

    Vector(size_t count, const T& defaultValue) {
        alloc_ = Alloc();
        data_ = alloc_.allocate(count);
        size_ = count;
        capacity_ = count;
        for (int i = 0; i < count; i++) {
            data_[i] = defaultValue;
        }
    }

    iterator begin() noexcept {
        return iterator(data_.get(), false);
    }
    iterator rbegin() noexcept {
        return iterator(data_.get() + size_ - 1, true);
    }

    iterator end() noexcept {
        return iterator(data_.get() + size_, false);
    }
    iterator rend() noexcept {
        return iterator(data_.get() - 1, true);
    }

    T operator[](size_t ind) {
        if ((ind >= size_) || (ind < 0)) {
            throw std::out_of_range("");
        }
        return data_[ind];
    }

    void push_back(value_type&& value) {
        if (size_ == capacity_) {
            auto new_data = alloc_.allocate(capacity_ * 2);
            std::copy(data_.get(), data_.get() + size_, new_data.get());
            data_.swap(new_data);
            data_[size_++] = value;
            capacity_ *= 2;
        } else if (size_ < capacity_) {
            data_[size_++] = value;
        } else {
            throw std::runtime_error("MY ERROR");
        }
    }

    void push_back(const value_type& value) {
        if (size_ == capacity_) {
            auto new_data = alloc_.allocate(capacity_ * 2);
            std::copy(data_.get(), data_.get() + size_, new_data.get());
            data_.swap(new_data);
            data_[size_++] = value;
            capacity_ *= 2;
        } else if (size_ < capacity_) {
            data_[size_++] = value;
        }
    }

    T pop_back() {
        if (size_ == 0) {
            throw std::runtime_error("nothing to pop");
        } else {
            return data_[--size_];
        }
    }

    void reserve(size_t count) {
        if (count > capacity_) {
            auto new_data = alloc_.allocate(count);
            std::copy(data_.get(), data_.get() + size_, new_data.get());
            data_.swap(new_data);
            capacity_ = count;
        }
    }

    void resize(size_t newSize) {
        if (newSize > capacity_) {
            auto new_data = alloc_.allocate(newSize);
            std::copy(data_.get(), data_.get() + size_, new_data.get());
            data_.swap(new_data);
            capacity_ = newSize;
            size_ = newSize;
        } else if (newSize > size_) {
            size_ = newSize;
        } else {
            for (int i = newSize; i < size_; i++) {
                data_[i] = T();
            }
            size_ = newSize;
        }
    }

    void resize(size_t newSize, const value_type& defaultValue) {
        if (newSize > capacity_) {
            auto new_data = alloc_.allocate(newSize);
            std::copy(data_.get(), data_.get() + size_, new_data.get());
            data_.swap(new_data);
            capacity_ = newSize;
            for (int i = size_; i < newSize; i++) {
                data_[i] = defaultValue;
            }
            size_ = newSize;
        } else {
            for (int i = size_; i < newSize; i++) {
                data_[i] = defaultValue;
            }
            size_ = newSize;
        }
    }

    bool empty() {
        return size_ == 0;
    }

    void clear() noexcept {
        data_ = alloc_.allocate(2);
        size_ = 0;
        capacity_ = 2;
    }

    size_t size() {
        return size_;
    }

    size_t capacity() {
        return capacity_;
    }

private:
    Alloc alloc_;
    size_t size_;
    size_t capacity_;
    std::unique_ptr<T[]> data_;
};

#endif //INC_07_VECTOR_H
