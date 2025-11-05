#include "rhombus.h"
#include <iostream>
#include <cmath>
#include <stdexcept>

Rhombus::Rhombus() : vertices(4) {}

Rhombus::Rhombus(const Point& center, double diagonal1, double diagonal2) : vertices(4) {
    vertices[0] = Point(center.x, center.y + diagonal2/2);
    vertices[1] = Point(center.x + diagonal1/2, center.y);
    vertices[2] = Point(center.x, center.y - diagonal2/2);
    vertices[3] = Point(center.x - diagonal1/2, center.y);
}

Rhombus::Rhombus(const std::vector<Point>& vertices) : vertices(vertices) {
    if (vertices.size() != 4) {
        throw std::invalid_argument("Rhombus must have exactly 4 vertices");
    }
}

Rhombus::Rhombus(const Rhombus& other) : vertices(other.vertices) {}

Rhombus::Rhombus(Rhombus&& other) noexcept : vertices(std::move(other.vertices)) {}

Rhombus& Rhombus::operator=(const Rhombus& other) {
    if (this != &other) {
        vertices = other.vertices;
    }
    return *this;
}

Rhombus& Rhombus::operator=(Rhombus&& other) noexcept {
    if (this != &other) {
        vertices = std::move(other.vertices);
    }
    return *this;
}

Point Rhombus::center() const {
    double x = 0, y = 0;
    for (const auto& vertex : vertices) {
        x += vertex.x;
        y += vertex.y;
    }
    return Point(x / 4, y / 4);
}

double Rhombus::area() const {
    double d1 = std::sqrt(std::pow(vertices[0].x - vertices[2].x, 2) + 
                         std::pow(vertices[0].y - vertices[2].y, 2));
    double d2 = std::sqrt(std::pow(vertices[1].x - vertices[3].x, 2) + 
                         std::pow(vertices[1].y - vertices[3].y, 2));
    return (d1 * d2) / 2;
}

void Rhombus::print(std::ostream& os) const {
    os << "Rhombus vertices: ";
    for (size_t i = 0; i < vertices.size(); ++i) {
        os << "(" << vertices[i].x << ", " << vertices[i].y << ")";
        if (i != vertices.size() - 1) os << " ";
    }
}

void Rhombus::read(std::istream& is) {
    vertices.resize(4);
    for (int i = 0; i < 4; ++i) {
        is >> vertices[i].x >> vertices[i].y;
    }
}

std::shared_ptr<Figure> Rhombus::clone() const {
    return std::make_shared<Rhombus>(*this);
}

bool Rhombus::operator==(const Figure& other) const {
    const Rhombus* otherRhombus = dynamic_cast<const Rhombus*>(&other);
    if (!otherRhombus) return false;
    
    for (size_t i = 0; i < vertices.size(); ++i) {
        if (!(vertices[i] == otherRhombus->vertices[i])) {
            return false;
        }
    }
    return true;
}

double Rhombus::get_diagonal1() const {
    return std::sqrt(std::pow(vertices[0].x - vertices[2].x, 2) + 
                    std::pow(vertices[0].y - vertices[2].y, 2));
}

double Rhombus::get_diagonal2() const {
    return std::sqrt(std::pow(vertices[1].x - vertices[3].x, 2) + 
                    std::pow(vertices[1].y - vertices[3].y, 2));
}