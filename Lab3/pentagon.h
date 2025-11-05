#ifndef PENTAGON_H
#define PENTAGON_H

#include "figure.h"
#include <vector>

class Pentagon : public Figure {
private:
    std::vector<Point> vertices;
    
public:
    Pentagon();
    Pentagon(const Point& center, double radius);
    Pentagon(const std::vector<Point>& vertices);
    
    Pentagon(const Pentagon& other);
    Pentagon(Pentagon&& other) noexcept;
    Pentagon& operator=(const Pentagon& other);
    Pentagon& operator=(Pentagon&& other) noexcept;
    ~Pentagon() override = default;
    
    Point center() const override;
    double area() const override;
    void print(std::ostream& os) const override;
    void read(std::istream& is) override;
    
    std::shared_ptr<Figure> clone() const override;
    bool operator==(const Figure& other) const override;
};

#endif