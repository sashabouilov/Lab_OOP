#ifndef FIGURE_ARRAY_H
#define FIGURE_ARRAY_H

#include <iostream>
#include <vector>
#include <memory>
#include "figure.h"

class FigureArray {
private:
    std::vector<std::shared_ptr<Figure>> figures;
    
public:
    void add(std::shared_ptr<Figure> figure) {
        figures.push_back(figure);
    }
    
    void remove(size_t index) {
        if (index < figures.size()) {
            figures.erase(figures.begin() + index);
        }
    }
    
    double total_area() const {
        double total = 0;
        for (const auto& figure : figures) {
            total += figure->area();
        }
        return total;
    }
    
    void print_all() const {
        for (size_t i = 0; i < figures.size(); ++i) {
            std::cout << "Figure " << i << ": " << *figures[i] 
                      << ", Area: " << figures[i]->area() 
                      << ", Center: (" << figures[i]->center().x 
                      << ", " << figures[i]->center().y << ")" << std::endl;
        }
    }
    
    size_t size() const {
        return figures.size();
    }
    
    std::shared_ptr<Figure> get(size_t index) const {
        if (index < figures.size()) {
            return figures[index];
        }
        return nullptr;
    }
};

#endif