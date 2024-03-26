#ifndef LABWORK_8_DARYAPANYUKOVA_ITERATOR_H
#define LABWORK_8_DARYAPANYUKOVA_ITERATOR_H

#include <iostream>

template<typename Type>
class Iterator : public std::iterator<std::random_access_iterator_tag, Type> {
public:
    typedef Type value_type;
    typedef Type& reference;
    typedef const Type& const_reference;
    typedef Iterator<Type> iterator;
    typedef Iterator<const Type> const_iterator;

    Iterator<value_type>() = default;

    Iterator<value_type>(value_type* begin, value_type* end,
                         value_type* head, value_type* tail, value_type* target)
            : begin_(begin), end_(end), head_(head),
              tail_(tail), target_(target) {};

    Iterator<value_type>(const Iterator<value_type>& other) :
            begin_(other.begin_), end_(other.end_), head_(other.head_),
            tail_(other.tail_), target_(other.target_) {};

    reference operator[](size_t index) const {
        return *(this + index);
    }

    void Swap(iterator& rhs) {
        std::swap(begin_, rhs.begin_);
        std::swap(end_, rhs.end_);
        std::swap(head_, rhs.head_);
        std::swap(tail_, rhs.tail_);
        std::swap(target_, rhs.target_);
    }

    iterator& operator=(iterator other) {
        if (this == &other) return *this;
        this->Swap(other);
        return *this;
    }

    reference operator*() const {
        return *target_;
    }

    iterator& operator++() {
        if (target_ == end_) {
            target_ = begin_;
        } else {
            ++target_;
        }
        return *this;
    }

    iterator operator++(int notused) {
        Iterator<Type> tmp = *this;
        ++(*this);
        return tmp;
    }

    iterator& operator--() {
        if (target_ == begin_) {
            target_ = end_;
        } else {
            --target_;
        }
        return *this;
    }

    iterator operator--(int notused) {
        iterator tmp = *this;
        --(*this);
        return tmp;
    }

    bool operator==(const iterator& other) const {
        return other.target_ == this->target_;
    }

    bool operator!=(const iterator& other) const {
        return !(*this == other);
    }

    bool operator<(const iterator& other) const {
        if (head_ <= tail_) {
            return this->target_ < other.target_;
        } else {
            if ((target_ > head_) == (other.target_ > other.head_)) {
                return other.target_ < this->target_;
            } else {
                return this->target_ > other.target_;
            }
        }
    }

    bool operator<=(const iterator& other) const {
        return !(this->target_ > other.target_);
    }

    bool operator>(const iterator& other) const {
        return other.target_ < this->target_;
    }

    bool operator>=(const iterator& other) const {
        return !(this->target_ < other.target_);
    }

    iterator operator+(size_t add) const {
        if (target_ + add > end_) {
            add -= (end_ - target_ + 1);
        }
        return iterator(begin_, end_, head_, tail_, target_ + add);
    }

    friend iterator operator+(size_t lhs, const iterator& rhs) {
        return rhs + lhs;
    }

    iterator& operator+=(size_t n) {
        *this = (*this + n);
        return *this;
    }

    iterator& operator-=(size_t n) {
        *this = (*this - n);
        return *this;
    }

    iterator operator-(size_t n) const {
        Iterator result = *this;
        if (target_ - n < begin_) {
            n -= (target_ - begin_ + 1);
            result.target_ = result.end_ - n;
        } else {
            result.target_ -= n;
        }
        return result;
    }

    size_t operator-(const iterator& right) const {
        if (this->target_ >= right.target_) {
            return this->target_ - right.target_;
        } else {
            return (this->end_ - this->target_) + 1 +
                   (right.target_ - right.begin_);
        }
    }

private:
    Type* begin_;
    Type* end_;
    Type* head_; // buffer begin
    Type* tail_; // buffer end
    Type* target_;
};


#endif //LABWORK_8_DARYAPANYUKOVA_ITERATOR_H
