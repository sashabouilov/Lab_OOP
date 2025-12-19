#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <cmath>

class Position {
public:
    double x, y;
    
    Position() : x(0), y(0) {}
    Position(double x, double y) : x(x), y(y) {}
    
    double distanceTo(const Position& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Position& pos) {
        os << "(" << pos.x << ", " << pos.y << ")";
        return os;
    }
};

#endif