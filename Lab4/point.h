#pragma once
#include <iostream>
#include <memory>
#include <concepts>
#include <cmath>


template<typename T>
concept Scalar = std::is_scalar_v<T>;


template<Scalar T>
class Point {
    friend std::ostream& operator<<(std::ostream& os, const Point<T>& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }

private:
    T x, y;

public:
    Point() : x(0), y(0) {}
    Point(T x, T y) : x(x), y(y) {}
    

    Point(const Point& other) : x(other.x), y(other.y) {}
    

    Point& operator=(const Point& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }
    

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
    

    T getX() const { return x; }
    T getY() const { return y; }
    

    void setX(T newX) { x = newX; }
    void setY(T newY) { y = newY; }
    

    double distance(const Point& other) const {
        return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
    }
};