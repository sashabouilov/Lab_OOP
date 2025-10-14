#include "Six.h"
#include <algorithm>
#include <stdexcept>
#include <cstring>

void Six::validateDigit(unsigned char digit) const {
    if (digit >= 6) {
        throw std::invalid_argument("Digit must be less than 6 for base-6 number");
    }
}

void Six::removeLeadingZeros() {

    while (_size > 1 && _array[_size - 1] == 0) {
        --_size;
    }
    
    if (_size == 0) {
        delete[] _array;
        _array = new unsigned char[1];
        _array[0] = 0;
        _size = 1;
        return;
    }
    
    unsigned char* newArray = new unsigned char[_size];
    std::copy(_array, _array + _size, newArray);
    delete[] _array;
    _array = newArray;
}

void Six::resize(size_t newSize) {
    if (newSize == _size) return;
    
    unsigned char* newArray = nullptr;

    if (newSize > 0) {
        newArray = new unsigned char[newSize];
        size_t minSize = std::min(_size, newSize);
        if (_array && newArray) {
            std::copy(_array, _array + minSize, newArray);
            if (newSize > _size) {
                std::fill(newArray + _size, newArray + newSize, 0);
            }
        }
    }
    
    delete[] _array;
    _array = newArray;
    _size = newSize;
}

Six::Six() : _size(1), _array(new unsigned char[1]) {
    _array[0] = 0;
}

Six::Six(const size_t& n, unsigned char t) : _size(n), _array(nullptr) {
    if (n == 0) {
        throw std::invalid_argument("Size cannot be zero");
    }
    validateDigit(t);
    _array = new unsigned char[n];
    std::fill(_array, _array + n, t);
}

Six::Six(const std::string& t) : _size(t.length()), _array(nullptr) {
    if (_size == 0) {
        throw std::invalid_argument("String cannot be empty");
    }
    
    _array = new unsigned char[_size];
    for (size_t i = 0; i < _size; ++i) {
        char c = t[_size - 1 - i];
        if (c < '0' || c > '5') {
            delete[] _array;
            throw std::invalid_argument("Invalid digit for base-6 number");
        }
        _array[i] = c - '0';
    }
    removeLeadingZeros();
}

Six::Six(const Six& other) : _size(other._size), _array(nullptr) {
    if (_size > 0) {
        _array = new unsigned char[_size];
        std::copy(other._array, other._array + _size, _array);
    }
}

Six::Six(Six&& other) noexcept : _size(other._size), _array(other._array) {
    other._size = 0;
    other._array = nullptr;
}

Six::~Six() noexcept {
    delete[] _array;
}

size_t Six::size() const {
    return _size;
}

std::string Six::toString() const {
    if (_size == 0) return "0";
    
    std::string result;
    for (int i = _size - 1; i >= 0; --i) {
        result += (_array[i] + '0');
    }
    return result;
}

Six Six::add(const Six& other) const {
    size_t maxSize = std::max(_size, other._size);
    Six result(maxSize + 1, 0);
    
    unsigned char carry = 0;
    for (size_t i = 0; i < maxSize || carry; ++i) {
        unsigned char sum = carry;
        if (i < _size) sum += _array[i];
        if (i < other._size) sum += other._array[i];
        
        result._array[i] = sum % 6;
        carry = sum / 6;
    }
    
    result.removeLeadingZeros();
    return result;
}

Six Six::subtract(const Six& other) const {
    if (lessThan(other)) {
        throw std::underflow_error("Subtraction would result in negative number");
    }
    
    Six result(*this);
    unsigned char borrow = 0;
    
    for (size_t i = 0; i < result._size; ++i) {
        int diff = result._array[i] - borrow;
        if (i < other._size) {
            diff -= other._array[i];
        }
        
        if (diff < 0) {
            diff += 6;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result._array[i] = diff;
    }
    
    result.removeLeadingZeros();
    return result;
}

Six Six::copy() const {
    return Six(*this);
}

bool Six::equals(const Six& other) const {
    if (_size != other._size) return false;
    for (size_t i = 0; i < _size; ++i) {
        if (_array[i] != other._array[i]) return false;
    }
    return true;
}

bool Six::greaterThan(const Six& other) const {
    if (_size != other._size) {
        return _size > other._size;
    }
    
    for (int i = _size - 1; i >= 0; --i) {
        if (_array[i] != other._array[i]) {
            return _array[i] > other._array[i];
        }
    }
    return false;
}

bool Six::lessThan(const Six& other) const {
    return !equals(other) && !greaterThan(other);
}

Six& Six::operator=(const Six& other) {
    if (this != &other) {
        delete[] _array;
        _size = other._size;
        if (_size > 0) {
            _array = new unsigned char[_size];
            std::copy(other._array, other._array + _size, _array);
        } else {
            _array = nullptr;
        }
    }
    return *this;
}

Six& Six::operator=(Six&& other) noexcept {
    if (this != &other) {
        delete[] _array;
        _size = other._size;
        _array = other._array;
        other._size = 0;
        other._array = nullptr;
    }
    return *this;
}