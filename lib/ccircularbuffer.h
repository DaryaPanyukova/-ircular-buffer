#pragma once

#include "abstractbuffer.h"

template<typename Type, typename alloc = std::allocator<Type>>
class CCircularBuffer : public AbstractBuffer<Type, alloc> {
public:
    typedef Type value_type;
    typedef Type& reference;
    typedef const Type& const_reference;
    typedef Iterator<Type> iterator;
    typedef Iterator<const Type> const_iterator;

    CCircularBuffer() : AbstractBuffer<Type, alloc>() {}

    explicit CCircularBuffer(const size_t size)
            : AbstractBuffer<value_type, alloc>(
            size) {}

    explicit CCircularBuffer(const AbstractBuffer<value_type, alloc>& other)
            : AbstractBuffer<value_type, alloc>(other) {}

    CCircularBuffer(size_t n, value_type t) :
            AbstractBuffer<value_type, alloc>(n, t) {}

    CCircularBuffer(const const_iterator& lhs, const const_iterator& rhs) :
            AbstractBuffer<value_type, alloc>(lhs, rhs) {}

    CCircularBuffer(const std::initializer_list<value_type>& list) :
            AbstractBuffer<value_type, alloc>(list) {}

    iterator
    insert(const const_iterator& p, const_reference t) override {
        if (this->head_ == this->tail_) {
            this->head_ = const_cast<value_type*>(&*p);
            this->tail_ = this->head_;
            this->size_ = 1;
            push_front(t);
            return this->begin();
        }

        iterator it = this->begin();
        push_front(*it);
        it = ++(this->begin());
        iterator stop(this->buffer_, this->buffer_ + this->capacity_,
                      this->head_, this->tail_, const_cast<value_type*>(&*p));
        for (; it != stop - 1; ++it) {
            *it = *(it + 1);
        }
        *it = t;
        return iterator(this->buffer_, this->buffer_ + this->capacity_,
                        this->head_, this->tail_, &*it);
    }

    iterator
    insert(const const_iterator& p, size_t n, const_reference t) override {
        iterator stop(this->buffer_, this->buffer_ + this->capacity_,
                      this->head_, this->tail_, const_cast<value_type*>(&*p));
        n = std::min(n, this->capacity_ - this->size_);
        if (n == 0) return stop;
        iterator it = this->begin();
        for (; it != stop; ++it) {
            std::allocator_traits<alloc>::construct(this->alloc_, &*(it - n),
                                                    *it);
        }

        for (it = stop - n; it != stop; ++it) {
            *it = t;
        }
        this->head_ = &*(this->begin() - n);
        this->size_ += n;
        return iterator(this->buffer_, this->buffer_ + this->capacity_,
                        this->head_, this->tail_, &*(it - n));
    }

    iterator
    insert(const const_iterator& p, const const_iterator& lhs,
           const const_iterator& rhs) override {
        size_t n = rhs - lhs;
        n = std::min(n, this->capacity_ - this->size_);
        iterator stop(this->buffer_, this->buffer_ + this->capacity_,
                      this->head_, this->tail_, const_cast<value_type*>(&*p));
        if (n == 0) {
            return stop;
        }
        iterator it = this->begin();
        for (; it != stop; ++it) {
            std::allocator_traits<alloc>::construct(this->alloc_, &*(it - n),
                                                    *it);
        }

        iterator other(this->buffer_, this->buffer_ + this->capacity_,
                       this->head_, this->tail_,
                       const_cast<value_type*>(&*lhs));
        for (it = stop - n; it != stop; ++it, ++other) {
            *it = *other;
        }
        this->head_ = &*(this->begin() - n);
        this->size_ += n;
        return iterator(this->buffer_, this->buffer_ + this->capacity_,
                        this->head_, this->tail_, &*(it - n));
    }

    iterator insert(const const_iterator& p,
                    const std::initializer_list<value_type>& list) override {
        size_t n = list.size();
        n = std::min(n, this->capacity_ - this->size_);
        iterator stop(this->buffer_, this->buffer_ + this->capacity_,
                      this->head_, this->tail_, const_cast<value_type*>(&*p));
        if (n == 0) return stop;
        iterator it = this->begin();
        for (; it != stop; ++it) {
            std::allocator_traits<alloc>::construct(this->alloc_, &*(it - n),
                                                    *it);
        }

        size_t index = 0;
        it = stop - n;

        for (const auto& elem: list) {
            *it = elem;
            ++it;
        }
        this->head_ = &*(this->begin() - n);
        this->size_ += n;
        return iterator(this->buffer_, this->buffer_ + this->capacity_,
                        this->head_, this->tail_, &*(it - n));
    }


    void push_front(const_reference t) override {
        --(this->head_);
        if (this->head_ < this->buffer_) {
            this->head_ = this->buffer_ + this->capacity_;
        }
        std::allocator_traits<alloc>::construct(this->alloc_, this->head_, t);
        if (this->tail_ == this->head_) {
            --(this->tail_);
            if (this->tail_ < this->buffer_) {
                this->tail_ = this->buffer_ + this->capacity_;
            }
        } else {
            ++(this->size_);
        }
        if (this->size_ == this->capacity_) {
            std::allocator_traits<alloc>::destroy(this->alloc_, this->tail_);
        }
    }

    void push_back(const_reference t) override {
        std::allocator_traits<alloc>::construct(this->alloc_, this->tail_, t);
        this->tail_++;
        if (this->tail_ > this->buffer_ + this->capacity_) {
            this->tail_ = this->buffer_;
        }

        if (this->tail_ == this->head_) {
            this->head_++;
            if (this->head_ > this->buffer_ + this->capacity_) {
                this->head_ = this->buffer_;
            }
        } else {
            this->size_++;
        }
        if (this->size_ == this->capacity_) {
            std::allocator_traits<alloc>::destroy(this->alloc_, this->tail_);
        }
    }
};

