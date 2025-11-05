#include "hexagon.h"
#include <iostream>
#include <cmath>
#include <stdexcept>

Hexagon::Hexagon() : vertices(6) {}

Hexagon::Hexagon(const Point& center, double radius) : vertices(6) {
    for (int i = 0; i < 6; ++i) {
        double angle = 2 * M_PI * i / 6;
        vertices[i] = Point(center.x + radius * std::cos(angle),
                           center.y + radius * std::sin(angle));
    }
}

Hexagon::Hexagon(const std::vector<Point>& vertices) : vertices(vertices) {
    if (vertices.size() != 6) {
        throw std::invalid_argument("Hexagon must have exactly 6 vertices");
    }
}

// Конструктор копирования
Hexagon::Hexagon(const Hexagon& other) : vertices(other.vertices) {}

// Конструктор перемещения
Hexagon::Hexagon(Hexagon&& other) noexcept : vertices(std::move(other.vertices)) {}

// Оператор присваивания копированием
Hexagon& Hexagon::operator=(const Hexagon& other) {
    if (this != &other) {
        vertices = other.vertices;
    }
    return *this;
}

// Оператор присваивания перемещением
Hexagon& Hexagon::operator=(Hexagon&& other) noexcept {
    if (this != &other) {
        vertices = std::move(other.vertices);
    }
    return *this;
}

Point Hexagon::center() const {
    double x = 0, y = 0;
    for (const auto& vertex : vertices) {
        x += vertex.x;
        y += vertex.y;
    }
    return Point(x / 6, y / 6);
}

double Hexagon::area() const {
    double side = std::sqrt(std::pow(vertices[0].x - vertices[1].x, 2) + 
                           std::pow(vertices[0].y - vertices[1].y, 2));
    return (3 * std::sqrt(3) / 2) * side * side;
}

void Hexagon::print(std::ostream& os) const {
    os << "Hexagon vertices: ";
    for (size_t i = 0; i < vertices.size(); ++i) {
        os << "(" << vertices[i].x << ", " << vertices[i].y << ")";
        if (i != vertices.size() - 1) os << " ";
    }
}

void Hexagon::read(std::istream& is) {
    vertices.resize(6);
    for (int i = 0; i < 6; ++i) {
        is >> vertices[i].x >> vertices[i].y;
    }
}

std::shared_ptr<Figure> Hexagon::clone() const {
    return std::make_shared<Hexagon>(*this);
}

bool Hexagon::operator==(const Figure& other) const {
    const Hexagon* otherHexagon = dynamic_cast<const Hexagon*>(&other);
    if (!otherHexagon) return false;
    
    for (size_t i = 0; i < vertices.size(); ++i) {
        if (!(vertices[i] == otherHexagon->vertices[i])) {
            return false;
        }
    }
    return true;
}