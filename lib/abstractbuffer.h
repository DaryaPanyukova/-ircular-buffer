#pragma once

#include "iterator.h"
#include <iostream>
#include <memory>


template<typename Type, typename alloc = std::allocator<Type>>
class AbstractBuffer {
public:
    typedef Type value_type;
    typedef Type& reference;
    typedef const Type& const_reference;
    typedef Iterator<Type> iterator;
    typedef Iterator<const Type> const_iterator;

    // --------------------  Container --------------------
    AbstractBuffer() : head_(nullptr), tail_(nullptr), buffer_(nullptr),
                       capacity_(0), size_(0) {};

    explicit AbstractBuffer(const size_t size) : capacity_(size), size_(0) {

        buffer_ = head_ = tail_ =
                std::allocator_traits<alloc>::allocate(alloc_, capacity_ + 1);
    }

    AbstractBuffer(const AbstractBuffer<value_type, alloc>& other) {
        capacity_ = other.capacity_;
        size_ = other.size_;

        buffer_ = std::allocator_traits<alloc>::allocate(alloc_,
                                                         other.capacity_ + 1);
        head_ = buffer_ + (other.head_ - other.buffer_);
        tail_ = buffer_ + (other.tail_ - other.buffer_);

        auto it = this->cbegin();
        auto end = this->cend();

        auto it_other = other.cbegin();
        for (; it != end; ++it, ++it_other) {
            std::allocator_traits<alloc>::construct(alloc_, &(*it), *it_other);
        }
    }

    AbstractBuffer& operator=(AbstractBuffer<value_type, alloc> other) {
        if (*this == other) {
            return *this;
        }
        clear();
        this->swap(other);
        return *this;
    }

    ~AbstractBuffer() {
        Delete();
    }

    [[nodiscard]] iterator begin() {
        return Iterator<value_type>(buffer_, (buffer_ + capacity_),
                                    head_, tail_, head_);
    }

    [[nodiscard]] iterator end() {
        return Iterator<value_type>(buffer_, (buffer_ + capacity_), head_,
                                    tail_,
                                    tail_);
    }

    [[nodiscard]] const_iterator cbegin() const {
        return Iterator<const value_type>(buffer_, (buffer_ + capacity_), head_,
                                          tail_, head_);

    }

    [[nodiscard]] const_iterator cend() const {
        return const_iterator(buffer_, (buffer_ + capacity_), head_,
                              tail_, tail_);

    }

    bool operator==(const AbstractBuffer<value_type, alloc>& other) const {
        if (this->size_ != other.size_) {
            return false;
        }

        const_iterator it = this->cbegin();
        const_iterator end = this->cend();

        const_iterator it_other = this->cbegin();
        const_iterator end_other = this->cend();

        while (it != end) {
            if (*it != *it_other) {
                return false;
            }
            it++;
            it_other++;
        }
        return true;
    }

    bool operator!=(const AbstractBuffer<value_type, alloc>& other) const {
        return !(*this == other);
    }

    void swap(AbstractBuffer<value_type, alloc>& other) {
        std::swap(this->head_, other.head_);
        std::swap(this->tail_, other.tail_);
        std::swap(this->capacity_, other.capacity_);
        std::swap(this->size_, other.size_);
        std::swap(this->buffer_, other.buffer_);
    }

    friend void
    swap(AbstractBuffer<value_type, alloc>& lhs,
         AbstractBuffer<value_type, alloc>& rhs);

    [[nodiscard]] size_t size() const {
        return size_;
    }

    [[nodiscard]] size_t capacity() const {
        return capacity_;
    }

    [[nodiscard]] size_t max_size() const {
        return alloc_.max_size();
    }

    [[nodiscard]] bool empty() const {
        return (size_ == 0);
    }



    // ----------------- SequenceContainer -----------------

    AbstractBuffer(size_t n, value_type t) {
        capacity_ = n;
        size_ = n;

        buffer_ = std::allocator_traits<alloc>::allocate(alloc_, capacity_ + 1);
        head_ = buffer_;
        tail_ = buffer_ + capacity_;

        auto stop = end();
        for (auto it = begin(); it != stop; ++it) {
            std::allocator_traits<alloc>::construct(alloc_, &(*it), t);
        }
    }

    AbstractBuffer(const const_iterator& lhs, const const_iterator& rhs) {
        //  if (lhs > rhs) undefined behavior

        capacity_ = rhs - lhs;
        size_ = capacity_;

        buffer_ = std::allocator_traits<alloc>::allocate(alloc_, capacity_ + 1);
        head_ = buffer_;
        tail_ = buffer_ + size_;

        auto stop = this->end();
        for (auto it = begin(), other = lhs;
             it != stop; ++it, ++other) {
            std::allocator_traits<alloc>::construct(alloc_, &(*it), *other);
        }
    }

    AbstractBuffer(const std::initializer_list<value_type>& list) {
        size_ = capacity_ = list.size();
        head_ = buffer_ = std::allocator_traits<alloc>::allocate(alloc_,
                                                                 capacity_ + 1);
        tail_ = head_ + capacity_;

        iterator it = this->begin();
        for (const value_type& value: list) {
            std::allocator_traits<alloc>::construct(alloc_, &(*it), value);
            ++it;
        }
    }

    AbstractBuffer& operator=(const std::initializer_list<value_type>& list) {
        clear();

        size_ = capacity_ = list.size();
        head_ = buffer_ =
                std::allocator_traits<alloc>::allocate(alloc_, capacity_ + 1);
        tail_ = head_ + capacity_;
        iterator it = this->begin();
        for (const value_type& value: list) {
            std::allocator_traits<alloc>::construct(alloc_,
                                                    &(*it), value);
            ++it;
        }
        return (*this);
    }

    virtual iterator insert(const const_iterator& p, const_reference t) = 0;

    virtual iterator
    insert(const const_iterator& p, size_t n, const_reference t) = 0;

    virtual iterator
    insert(const const_iterator& p, const const_iterator& lhs,
           const const_iterator& rhs) = 0;

    virtual iterator
    insert(const const_iterator& p, const std::initializer_list<value_type>& list) = 0;

    iterator erase(const_iterator q) {
        if (size_ == 0 || !(q >= cbegin() && q < cend())) {
            throw std::out_of_range("out of range in call erase(iterator)");
        }

        std::allocator_traits<alloc>::destroy(alloc_, &*q);
        if (size_ == 1) {
            head_ = tail_ = buffer_;
            size_ = 0;
            return begin();
        }

        auto stop = this->end();
        Iterator<value_type> prev(buffer_, buffer_ + capacity_,
                                  head_, tail_, const_cast<int*>(&*q));
        std::allocator_traits<alloc>::construct(alloc_, &*prev, *(prev + 1));
        prev++;
        for (iterator it = prev + 1; it != stop; ++it, ++prev) {
            *prev = *it;
        }
        std::allocator_traits<alloc>::destroy(alloc_, tail_);
        tail_ = &*(--end());
        --size_;
        return iterator(buffer_, buffer_ + capacity_, head_, tail_,
                        const_cast<int*>(&*(q + 1)));
    }

    iterator erase(const const_iterator& lhs, const const_iterator& rhs) {
        size_t n = rhs - lhs;
        if (size_ < n || !(lhs >= cbegin() && lhs < rhs && rhs < cend())) {
            throw std::out_of_range(
                    "out of range in call erase(iterator, iterator)");
        }

        auto stop = this->end();
        Iterator<value_type> right(buffer_, buffer_ + capacity_, head_, tail_,
                                   const_cast<value_type*>(&*rhs));
        Iterator<value_type> left(buffer_, buffer_ + capacity_, head_, tail_,
                                  const_cast<value_type*>(&*lhs));
        for (; right != stop; ++right, ++left) {
            *left = *right;
            if (right >= stop - n) {
                std::allocator_traits<alloc>::destroy(alloc_, &*right);
            }
        }
        tail_ = &*(end() - n);
        size_ -= n;

        return iterator(buffer_, buffer_ + capacity_, head_,
                        tail_, const_cast<value_type*>(&*rhs));
    }

    void clear() {
        for (auto it = this->begin(); it != this->end(); ++it) {
            std::allocator_traits<alloc>::destroy(alloc_, &(*it));
        }
        head_ = tail_ = buffer_;
        size_ = 0;
    }

    void assign(const const_iterator& lhs, const const_iterator& rhs) {
        Delete();

        capacity_ = rhs - lhs;
        size_ = capacity_;

        buffer_ = std::allocator_traits<alloc>::allocate(alloc_, capacity_ + 1);
        head_ = buffer_;
        tail_ = buffer_ + size_;

        auto end = this->cend();
        for (auto it = begin(), other = lhs; it != end; ++it, ++other) {
            std::allocator_traits<alloc>::construct(alloc_, &(*it), *other);
        }
    }

    void assign(const std::initializer_list<value_type>& list) {
        (*this) = list;
    }

    void assign(size_t n, value_type t) {
        Delete();
        capacity_ = n;
        size_ = n;

        buffer_ = std::allocator_traits<alloc>::allocate(alloc_, capacity_ + 1);
        head_ = buffer_;
        tail_ = buffer_ + n;

        auto stop = this->end();
        for (auto it = begin(); it != stop; ++it) {
            std::allocator_traits<alloc>::construct(alloc_, &(*it), t);
        }
    }

    // --------------------- optional ---------------------

    reference front() {
        return *begin();
    }

    reference back() {
        auto tmp = end();
        --tmp;
        return *tmp;
    }

    virtual void push_front(const_reference t) = 0;

    virtual void push_back(const_reference t) = 0;

    void pop_front() {
        --size_;
        std::allocator_traits<alloc>::destroy(alloc_, head_);

        head_++;
        if (head_ > buffer_ + capacity_) {
            head_ = buffer_;
        }
        if (head_ == tail_) {
            head_ = tail_ = buffer_;
        }
    }

    void pop_back() {
        --size_;
        --tail_;
        if (tail_ < buffer_) {
            tail_ = buffer_ + capacity_;
        }

        std::allocator_traits<alloc>::destroy(alloc_, tail_);
        if (head_ == tail_) {
            head_ = tail_ = buffer_;
        }
    }

    reference operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("out of range in call operator[]");
        }

        if (head_ + index <= buffer_ + capacity_ || head_ < tail_) {
            return *(head_ + index);
        } else {
            size_t left = index - ((buffer_ + capacity_) - head_ + 1);
            return *(buffer_ + left);
        }
    }

    // ------------------------------------------------------
    void resize(size_t count) {
        if (count == size_) return;
        if (count < size_) {
            for (auto it = begin() + count; it != end(); ++it) {
                std::allocator_traits<alloc>::destroy(alloc_, &*it);
            }
            size_ = count;
            tail_ = &*begin() + count;
        } else if (count > capacity_) {
            value_type* new_buffer =
                    std::allocator_traits<alloc>::allocate(alloc_, count + 1);
            value_type* tmp = new_buffer;
            for (const auto& elem: *this) {
                std::allocator_traits<alloc>::construct(alloc_, tmp, elem);
                tmp++;
            }
            for (size_t i = 0; i < count - size_; ++i, ++tmp) {
                std::allocator_traits<alloc>::construct(alloc_, tmp);
            }
            Delete();
            buffer_ = new_buffer;
            head_ = new_buffer;
            tail_ = head_ + count;
            capacity_ = count;
            size_ = count;
        } else {
            value_type* tmp = &*end();
            for (size_t i = 0; i < count - size_; ++i, ++tmp) {
                std::allocator_traits<alloc>::construct(alloc_, tmp);
            }
            tail_ = head_ + count;
            size_ = count;
        }
    }

    void resize(size_t count, const_reference value) {
        if (count == size_) return;
        if (count < size_) {
            for (auto it = begin() + count; it != end(); ++it) {
                std::allocator_traits<alloc>::destroy(alloc_, &*it);
            }
            size_ = count;
            tail_ = &*begin() + count;
        } else if (count > capacity_) {
            value_type* new_buffer =
                    std::allocator_traits<alloc>::allocate(alloc_, count + 1);
            value_type* tmp = new_buffer;
            for (const auto& elem: *this) {
                std::allocator_traits<alloc>::construct(alloc_, tmp, elem);
                tmp++;
            }
            for (size_t i = 0; i < count - size_; ++i, ++tmp) {
                std::allocator_traits<alloc>::construct(alloc_, tmp, value);
            }
            Delete();
            buffer_ = new_buffer;
            head_ = new_buffer;
            tail_ = head_ + count;
            capacity_ = count;
            size_ = count;
        } else {
            value_type* tmp = &*end();
            for (size_t i = 0; i < count - size_; ++i, ++tmp) {
                std::allocator_traits<alloc>::construct(alloc_, tmp, value);
            }
            tail_ = head_ + count;
            size_ = count;
        }
    }

    void reserve(size_t count) {
        if (count < capacity_) return;

        value_type* new_buffer =
                std::allocator_traits<alloc>::allocate(alloc_, count + 1);
        value_type* tmp = new_buffer;
        for (const auto& elem: *this) {
            std::allocator_traits<alloc>::construct(alloc_, tmp, elem);
            tmp++;
        }
        size_t tmp_size = size_;
        Delete();
        capacity_ = count;
        size_ = tmp_size;
        buffer_ = new_buffer;
        head_ = new_buffer;
        tail_ = head_ + size_;
    }

protected:
    void Delete() {
        for (auto it = this->begin(); it != this->end(); ++it) {
            std::allocator_traits<alloc>::destroy(alloc_, &(*it));
        }
        std::allocator_traits<alloc>::deallocate(alloc_, buffer_,
                                                 capacity_ + 1);

        buffer_ = head_ = tail_ = nullptr;
        size_ = capacity_ = 0;
    }

    alloc alloc_;
    Type* head_;
    Type* tail_;
    size_t capacity_;
    size_t size_;
    Type* buffer_;
};

template<typename Type, typename alloc = std::allocator<Type>>
void
swap(AbstractBuffer<Type, alloc>& lhs, AbstractBuffer<Type, alloc>& rhs) {
    lhs.swap(rhs);
}