#include <iostream>
#include <memory>
#include <limits>
#include "point.h"
#include "figure.h"
#include "array.h"


template<Scalar T>
std::shared_ptr<Figure<T>> inputSquare() {
    T x, y, radius;
    std::cout << "Enter square center (x y): ";
    std::cin >> x >> y;
    std::cout << "Enter radius (circumradius): ";
    std::cin >> radius;
    return std::make_shared<Square<T>>(x, y, radius);
}


template<Scalar T>
std::shared_ptr<Figure<T>> inputRectangle() {
    T x, y, radius, aspectRatio;
    std::cout << "Enter rectangle center (x y): ";
    std::cin >> x >> y;
    std::cout << "Enter radius (circumradius): ";
    std::cin >> radius;
    std::cout << "Enter aspect ratio (width/height): ";
    std::cin >> aspectRatio;
    return std::make_shared<Rectangle<T>>(x, y, radius, aspectRatio);
}


template<Scalar T>
std::shared_ptr<Figure<T>> inputTrapezoid() {
    T x, y, radius, topBaseRatio;
    std::cout << "Enter trapezoid center (x y): ";
    std::cin >> x >> y;
    std::cout << "Enter radius (circumradius): ";
    std::cin >> radius;
    std::cout << "Enter top base ratio (0-1): ";
    std::cin >> topBaseRatio;
    return std::make_shared<Trapezoid<T>>(x, y, radius, topBaseRatio);
}


template<Scalar T>
void printAllFigures(const Array<std::shared_ptr<Figure<T>>>& figures) {
    std::cout << "\n=== All Figures ===" << std::endl;
    for (size_t i = 0; i < figures.size(); ++i) {
        std::cout << "Figure " << i << ": ";
        figures[i]->printInfo(std::cout);
        std::cout << std::endl;
        std::cout << "  ";
        figures[i]->printVertices(std::cout);
        std::cout << std::endl << std::endl;
    }
}

template<Scalar T>
double totalArea(const Array<std::shared_ptr<Figure<T>>>& figures) {
    double total = 0.0;
    for (size_t i = 0; i < figures.size(); ++i) {
        total += figures[i]->area();
    }
    return total;
}


void demonstrateArrayTypes() {
    std::cout << "\n=== Demonstrating Array with different types ===" << std::endl;
    

    Array<std::shared_ptr<Figure<double>>> figureArray;
    figureArray.push_back(std::make_shared<Square<double>>(0, 0, 5));
    figureArray.push_back(std::make_shared<Rectangle<double>>(1, 1, 3, 1.5));
    
    std::cout << "Figure array size: " << figureArray.size() << std::endl;
    

    Array<std::shared_ptr<Square<double>>> squareArray;
    squareArray.push_back(std::make_shared<Square<double>>(0, 0, 4));
    squareArray.push_back(std::make_shared<Square<double>>(2, 2, 3));
    
    std::cout << "Square array size: " << squareArray.size() << std::endl;
    

    squareArray.resize_elements(5, std::make_shared<Square<double>>(0, 0, 1));
    std::cout << "After resize - square array size: " << squareArray.size() << std::endl;
    

    Array<std::shared_ptr<Square<double>>> movedArray = std::move(squareArray);
    std::cout << "After move - original size: " << squareArray.size() 
              << ", moved size: " << movedArray.size() << std::endl;
}

int main() {
    Array<std::shared_ptr<Figure<double>>> figures;
    
    std::cout << "=== Geometric Figures Management System ===" << std::endl;
    std::cout << "All figures are inscribed in a circle (rotational or circumscribed)" << std::endl;
    
    while (true) {
        std::cout << "\nMenu:" << std::endl;
        std::cout << "1. Add Square" << std::endl;
        std::cout << "2. Add Rectangle" << std::endl;
        std::cout << "3. Add Trapezoid" << std::endl;
        std::cout << "4. Print all figures" << std::endl;
        std::cout << "5. Calculate total area" << std::endl;
        std::cout << "6. Remove figure by index" << std::endl;
        std::cout << "7. Demonstrate array types" << std::endl;
        std::cout << "8. Exit" << std::endl;
        std::cout << "Choose option: ";
        
        int choice;
        std::cin >> choice;
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        try {
            switch (choice) {
                case 1:
                    figures.push_back(inputSquare<double>());
                    std::cout << "Square added successfully." << std::endl;
                    break;
                    
                case 2:
                    figures.push_back(inputRectangle<double>());
                    std::cout << "Rectangle added successfully." << std::endl;
                    break;
                    
                case 3:
                    figures.push_back(inputTrapezoid<double>());
                    std::cout << "Trapezoid added successfully." << std::endl;
                    break;
                    
                case 4:
                    if (figures.empty()) {
                        std::cout << "No figures to display." << std::endl;
                    } else {
                        printAllFigures(figures);
                    }
                    break;
                    
                case 5:
                    if (figures.empty()) {
                        std::cout << "No figures to calculate area." << std::endl;
                    } else {
                        std::cout << "Total area of all figures: " << totalArea(figures) << std::endl;
                    }
                    break;
                    
                case 6:
                    if (figures.empty()) {
                        std::cout << "No figures to remove." << std::endl;
                    } else {
                        std::cout << "Enter index to remove (0-" << figures.size()-1 << "): ";
                        size_t index;
                        std::cin >> index;
                        figures.remove(index);
                        std::cout << "Figure removed successfully." << std::endl;
                    }
                    break;
                    
                case 7:
                    demonstrateArrayTypes();
                    break;
                    
                case 8:
                    std::cout << "Goodbye!" << std::endl;
                    return 0;
                    
                default:
                    std::cout << "Invalid option. Please try again." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    
    return 0;
}