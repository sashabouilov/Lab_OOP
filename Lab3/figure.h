#ifndef FIGURE_H
#define FIGURE_H

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>


struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
    bool operator==(const Point& other) const {
        return std::abs(x - other.x) < 1e-9 && std::abs(y - other.y) < 1e-9;
    }
};


class Figure {
public:
    virtual ~Figure() = default;
    
    virtual Point center() const = 0;
    virtual double area() const = 0;
    virtual void print(std::ostream& os) const = 0;
    virtual void read(std::istream& is) = 0;
    
    virtual std::shared_ptr<Figure> clone() const = 0;
    virtual bool operator==(const Figure& other) const = 0;
    
    friend std::ostream& operator<<(std::ostream& os, const Figure& figure);
    friend std::istream& operator>>(std::istream& is, Figure& figure);
    operator double() const { return area(); }
};

#endif