#include <iostream>
#include "figure_array.h"
#include "rhombus.h"
#include "pentagon.h"
#include "hexagon.h"

int main() {
    FigureArray array;
    int choice;
    
    std::cout << "Figure Management System" << std::endl;
    std::cout << "1. Add Rhombus" << std::endl;
    std::cout << "2. Add Pentagon" << std::endl;
    std::cout << "3. Add Hexagon" << std::endl;
    std::cout << "4. Remove Figure" << std::endl;
    std::cout << "5. Print All Figures" << std::endl;
    std::cout << "6. Calculate Total Area" << std::endl;
    std::cout << "0. Exit" << std::endl;
    
    while (true) {
        std::cout << "\nEnter your choice: ";
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                auto rhombus = std::make_shared<Rhombus>();
                std::cout << "Enter 4 vertices (x y) for rhombus: ";
                std::cin >> *rhombus;
                array.add(rhombus);
                std::cout << "Rhombus added successfully!" << std::endl;
                break;
            }
            case 2: {
                auto pentagon = std::make_shared<Pentagon>();
                std::cout << "Enter 5 vertices (x y) for pentagon: ";
                std::cin >> *pentagon;
                array.add(pentagon);
                std::cout << "Pentagon added successfully!" << std::endl;
                break;
            }
            case 3: {
                auto hexagon = std::make_shared<Hexagon>();
                std::cout << "Enter 6 vertices (x y) for hexagon: ";
                std::cin >> *hexagon;
                array.add(hexagon);
                std::cout << "Hexagon added successfully!" << std::endl;
                break;
            }
            case 4: {
                size_t index;
                std::cout << "Enter index to remove: ";
                std::cin >> index;
                if (index < array.size()) {
                    array.remove(index);
                    std::cout << "Figure removed successfully!" << std::endl;
                } else {
                    std::cout << "Invalid index!" << std::endl;
                }
                break;
            }
            case 5:
                std::cout << "\nAll figures:" << std::endl;
                array.print_all();
                break;
            case 6:
                std::cout << "Total area of all figures: " << array.total_area() << std::endl;
                break;
            case 0:
                std::cout << "Goodbye!" << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
    }
    
    return 0;
}