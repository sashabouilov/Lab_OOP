#pragma once
#include "point.h"
#include <memory>
#include <vector>
#include <iostream>


template<Scalar T>
class Figure {
public:
    virtual ~Figure() = default;
    
    virtual Point<T> geometricCenter() const = 0;
    virtual double area() const = 0;
    virtual void printVertices(std::ostream& os) const = 0;
    virtual std::unique_ptr<Figure<T>> clone() const = 0;
    
    explicit operator double() const {
        return area();
    }
    
    virtual bool operator==(const Figure<T>& other) const = 0;
    bool operator!=(const Figure<T>& other) const {
        return !(*this == other);
    }
    
    virtual void printInfo(std::ostream& os) const {
        os << "Center: " << geometricCenter() << ", Area: " << area();
    }
};


template<Scalar T>
class RotationalFigure : public Figure<T> {
protected:
    std::unique_ptr<Point<T>> center;
    T radius;
    
public:
    RotationalFigure(T centerX, T centerY, T radius) 
        : center(std::make_unique<Point<T>>(centerX, centerY)), radius(radius) {}
    
    RotationalFigure(const RotationalFigure& other) 
        : center(std::make_unique<Point<T>>(*other.center)), radius(other.radius) {}
    
    RotationalFigure& operator=(const RotationalFigure& other) {
        if (this != &other) {
            center = std::make_unique<Point<T>>(*other.center);
            radius = other.radius;
        }
        return *this;
    }
    
    Point<T> geometricCenter() const override {
        return *center;
    }
    
    T getRadius() const { return radius; }
    void setRadius(T newRadius) { radius = newRadius; }
};


template<Scalar T>
class Square : public RotationalFigure<T> {
public:
    Square(T centerX, T centerY, T radius) 
        : RotationalFigure<T>(centerX, centerY, radius) {}

    Square(const Square& other) : RotationalFigure<T>(other) {}
    
    double area() const override {
        return 2.0 * this->radius * this->radius;
    }
    
    void printVertices(std::ostream& os) const override {
        T side = this->radius * std::sqrt(2.0);
        T halfSide = side / 2.0;
        
        std::vector<Point<T>> vertices = {
            {this->center->getX() - halfSide, this->center->getY() - halfSide},
            {this->center->getX() + halfSide, this->center->getY() - halfSide},
            {this->center->getX() + halfSide, this->center->getY() + halfSide},
            {this->center->getX() - halfSide, this->center->getY() + halfSide}
        };
        
        os << "Square vertices: ";
        for (size_t i = 0; i < vertices.size(); ++i) {
            os << vertices[i];
            if (i < vertices.size() - 1) os << ", ";
        }
    }
    
    std::unique_ptr<Figure<T>> clone() const override {
        return std::make_unique<Square<T>>(*this);
    }
    
    bool operator==(const Figure<T>& other) const override {
        const Square* otherSquare = dynamic_cast<const Square*>(&other);
        if (!otherSquare) return false;
        return *this->center == otherSquare->geometricCenter() && 
               this->radius == otherSquare->getRadius();
    }
    
    void printInfo(std::ostream& os) const override {
        os << "Square - ";
        Figure<T>::printInfo(os);
    }
};


template<Scalar T>
class Rectangle : public RotationalFigure<T> {
private:
    T aspectRatio;
    
public:
    Rectangle(T centerX, T centerY, T radius, T aspectRatio = 1.0) 
        : RotationalFigure<T>(centerX, centerY, radius), aspectRatio(aspectRatio) {}
    
    Rectangle(const Rectangle& other) 
        : RotationalFigure<T>(other), aspectRatio(other.aspectRatio) {}
    
    double area() const override {
        T width = 2 * this->radius * std::sqrt(aspectRatio / (1 + aspectRatio * aspectRatio));
        T height = width / aspectRatio;
        return width * height;
    }
    
    void printVertices(std::ostream& os) const override {
        T width = 2 * this->radius * std::sqrt(aspectRatio / (1 + aspectRatio * aspectRatio));
        T height = width / aspectRatio;
        T halfWidth = width / 2.0;
        T halfHeight = height / 2.0;
        
        std::vector<Point<T>> vertices = {
            {this->center->getX() - halfWidth, this->center->getY() - halfHeight},
            {this->center->getX() + halfWidth, this->center->getY() - halfHeight},
            {this->center->getX() + halfWidth, this->center->getY() + halfHeight},
            {this->center->getX() - halfWidth, this->center->getY() + halfHeight}
        };
        
        os << "Rectangle vertices: ";
        for (size_t i = 0; i < vertices.size(); ++i) {
            os << vertices[i];
            if (i < vertices.size() - 1) os << ", ";
        }
    }
    
    std::unique_ptr<Figure<T>> clone() const override {
        return std::make_unique<Rectangle<T>>(*this);
    }
    
    bool operator==(const Figure<T>& other) const override {
        const Rectangle* otherRect = dynamic_cast<const Rectangle*>(&other);
        if (!otherRect) return false;
        return *this->center == otherRect->geometricCenter() && 
               this->radius == otherRect->getRadius() &&
               aspectRatio == otherRect->aspectRatio;
    }
    
    T getAspectRatio() const { return aspectRatio; }
    
    void printInfo(std::ostream& os) const override {
        os << "Rectangle (aspect: " << aspectRatio << ") - ";
        Figure<T>::printInfo(os);
    }
};


template<Scalar T>
class Trapezoid : public RotationalFigure<T> {
private:
    T topBaseRatio;
    
public:
    Trapezoid(T centerX, T centerY, T radius, T topBaseRatio = 0.5) 
        : RotationalFigure<T>(centerX, centerY, radius), topBaseRatio(topBaseRatio) {
        if (topBaseRatio <= 0 || topBaseRatio >= 1) {
            throw std::invalid_argument("Top base ratio must be between 0 and 1");
        }
    }
    
    Trapezoid(const Trapezoid& other) 
        : RotationalFigure<T>(other), topBaseRatio(other.topBaseRatio) {}
    
    double area() const override {
        T bottomBase = 2 * this->radius;
        T topBase = bottomBase * topBaseRatio;
        T height = this->radius * std::sqrt(1 - topBaseRatio * topBaseRatio);
        return (topBase + bottomBase) * height / 2.0;
    }
    
    void printVertices(std::ostream& os) const override {
        T bottomBase = 2 * this->radius;
        T topBase = bottomBase * topBaseRatio;
        T height = this->radius * std::sqrt(1 - topBaseRatio * topBaseRatio);
        
        T bottomHalf = bottomBase / 2.0;
        T topHalf = topBase / 2.0;
        
        std::vector<Point<T>> vertices = {
            {this->center->getX() - bottomHalf, this->center->getY() - height/2},
            {this->center->getX() + bottomHalf, this->center->getY() - height/2},
            {this->center->getX() + topHalf, this->center->getY() + height/2},
            {this->center->getX() - topHalf, this->center->getY() + height/2}
        };
        
        os << "Trapezoid vertices: ";
        for (size_t i = 0; i < vertices.size(); ++i) {
            os << vertices[i];
            if (i < vertices.size() - 1) os << ", ";
        }
    }
    
    std::unique_ptr<Figure<T>> clone() const override {
        return std::make_unique<Trapezoid<T>>(*this);
    }
    
    bool operator==(const Figure<T>& other) const override {
        const Trapezoid* otherTrap = dynamic_cast<const Trapezoid*>(&other);
        if (!otherTrap) return false;
        return *this->center == otherTrap->geometricCenter() && 
               this->radius == otherTrap->getRadius() &&
               topBaseRatio == otherTrap->topBaseRatio;
    }
    
    T getTopBaseRatio() const { return topBaseRatio; }
    
    void printInfo(std::ostream& os) const override {
        os << "Trapezoid (top/base: " << topBaseRatio << ") - ";
        Figure<T>::printInfo(os);
    }
};