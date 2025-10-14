#ifndef SIX_H
#define SIX_H

#include <string>
#include <stdexcept>

class Six {
private:
    size_t _size;
    unsigned char* _array;
    
    void validateDigit(unsigned char digit) const;
    void removeLeadingZeros();
    void resize(size_t newSize);
    
public:
    Six();
    explicit Six(const size_t& n, unsigned char t = 0);
    explicit Six(const std::string& t);
    Six(const Six& other);
    Six(Six&& other) noexcept;

    ~Six() noexcept;
    
    size_t size() const;
    std::string toString() const;

    Six add(const Six& other) const;
    Six subtract(const Six& other) const;
    Six copy() const;

    bool equals(const Six& other) const;
    bool greaterThan(const Six& other) const;
    bool lessThan(const Six& other) const;

    Six& operator=(const Six& other);
    Six& operator=(Six&& other) noexcept;
};

#endif