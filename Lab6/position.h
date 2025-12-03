#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <cmath>

struct Position {
    double x, y;
    
    Position(double x = 0, double y = 0) : x(x), y(y) {}
    
    double distanceTo(const Position& other) const {
        return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Position& pos) {
        os << "(" << pos.x << ", " << pos.y << ")";
        return os;
    }
};

#endif