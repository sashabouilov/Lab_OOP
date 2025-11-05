#include "pentagon.h"
#include <iostream>
#include <cmath>
#include <stdexcept>

Pentagon::Pentagon() : vertices(5) {}

Pentagon::Pentagon(const Point& center, double radius) : vertices(5) {
    for (int i = 0; i < 5; ++i) {
        double angle = 2 * M_PI * i / 5;
        vertices[i] = Point(center.x + radius * std::cos(angle),
                           center.y + radius * std::sin(angle));
    }
}

Pentagon::Pentagon(const std::vector<Point>& vertices) : vertices(vertices) {
    if (vertices.size() != 5) {
        throw std::invalid_argument("Pentagon must have exactly 5 vertices");
    }
}

Pentagon::Pentagon(const Pentagon& other) : vertices(other.vertices) {}

Pentagon::Pentagon(Pentagon&& other) noexcept : vertices(std::move(other.vertices)) {}

Pentagon& Pentagon::operator=(const Pentagon& other) {
    if (this != &other) {
        vertices = other.vertices;
    }
    return *this;
}

Pentagon& Pentagon::operator=(Pentagon&& other) noexcept {
    if (this != &other) {
        vertices = std::move(other.vertices);
    }
    return *this;
}

Point Pentagon::center() const {
    double x = 0, y = 0;
    for (const auto& vertex : vertices) {
        x += vertex.x;
        y += vertex.y;
    }
    return Point(x / 5, y / 5);
}


double Pentagon::area() const {
    double side = std::sqrt(std::pow(vertices[0].x - vertices[1].x, 2) + 
                           std::pow(vertices[0].y - vertices[1].y, 2));
    return (1.0/4.0) * std::sqrt(5*(5+2*std::sqrt(5))) * side * side;
}


void Pentagon::print(std::ostream& os) const {
    os << "Pentagon vertices: ";
    for (size_t i = 0; i < vertices.size(); ++i) {
        os << "(" << vertices[i].x << ", " << vertices[i].y << ")";
        if (i != vertices.size() - 1) os << " ";
    }
}


void Pentagon::read(std::istream& is) {
    vertices.resize(5);
    for (int i = 0; i < 5; ++i) {
        is >> vertices[i].x >> vertices[i].y;
    }
}

std::shared_ptr<Figure> Pentagon::clone() const {
    return std::make_shared<Pentagon>(*this);
}


bool Pentagon::operator==(const Figure& other) const {
    const Pentagon* otherPentagon = dynamic_cast<const Pentagon*>(&other);
    if (!otherPentagon) return false;
    
    for (size_t i = 0; i < vertices.size(); ++i) {
        if (!(vertices[i] == otherPentagon->vertices[i])) {
            return false;
        }
    }
    return true;
}