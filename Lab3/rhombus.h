#ifndef RHOMBUS_H
#define RHOMBUS_H

#include "figure.h"
#include <vector>

class Rhombus : public Figure {
private:
    std::vector<Point> vertices;
    
public:
    Rhombus();
    Rhombus(const Point& center, double diagonal1, double diagonal2);
    Rhombus(const std::vector<Point>& vertices);
    
    Rhombus(const Rhombus& other);
    Rhombus(Rhombus&& other) noexcept;
    Rhombus& operator=(const Rhombus& other);
    Rhombus& operator=(Rhombus&& other) noexcept;
    ~Rhombus() override = default;
    
    Point center() const override;
    double area() const override;
    void print(std::ostream& os) const override;
    void read(std::istream& is) override;
    
    std::shared_ptr<Figure> clone() const override;
    bool operator==(const Figure& other) const override;
    
    double get_diagonal1() const;
    double get_diagonal2() const;
};

#endif