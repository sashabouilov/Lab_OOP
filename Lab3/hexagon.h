#ifndef HEXAGON_H
#define HEXAGON_H

#include "figure.h"
#include <vector>

class Hexagon : public Figure {
private:
    std::vector<Point> vertices;
    
public:
    Hexagon();
    Hexagon(const Point& center, double radius);
    Hexagon(const std::vector<Point>& vertices);
 
    Hexagon(const Hexagon& other);
    Hexagon(Hexagon&& other) noexcept;
    Hexagon& operator=(const Hexagon& other);
    Hexagon& operator=(Hexagon&& other) noexcept;
    ~Hexagon() override = default;
    
    Point center() const override;
    double area() const override;
    void print(std::ostream& os) const override;
    void read(std::istream& is) override;
    
    std::shared_ptr<Figure> clone() const override;
    bool operator==(const Figure& other) const override;
};

#endif