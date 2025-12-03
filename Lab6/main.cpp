#include "dungeon_editor.h"
#include <iostream>


void printMenu() {
    std::cout << "\nDungeon Editor" << std::endl;
    std::cout << "1. Add NPC" << std::endl;
    std::cout << "2. Print all NPCs" << std::endl;
    std::cout << "3. Save to file" << std::endl;
    std::cout << "4. Load from file" << std::endl;
    std::cout << "5. Start battle" << std::endl;
    std::cout << "6. Create demo NPCs" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choice: ";
}


int main() {
    DungeonEditor editor;
    int choice;
    
    do {
        printMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                std::string type, name;
                double x, y;
                
                std::cout << "NPC type (SlaveTrader/Squirrel/Knight): ";
                std::cin >> type;
                std::cout << "NPC name: ";
                std::cin >> name;
                std::cout << "X coordinate: ";
                std::cin >> x;
                std::cout << "Y coordinate: ";
                std::cin >> y;
                
                editor.addNPC(type, name, x, y);
                break;
            }
            
            case 2:
                editor.printNPCs();
                break;
                
            case 3: {
                std::string filename;
                std::cout << "Filename to save: ";
                std::cin >> filename;
                editor.saveToFile(filename);
                break;
            }
                
            case 4: {
                std::string filename;
                std::cout << "Filename to load: ";
                std::cin >> filename;
                editor.loadFromFile(filename);
                break;
            }
                
            case 5: {
                double range;
                std::cout << "Battle range: ";
                std::cin >> range;
                editor.startBattle(range);
                break;
            }
                
            case 6:
                editor.createDemoNPCs();
                break;
                
            case 0:
                break;
                
            default:
                std::cout << "Invalid choice" << std::endl;
        }
        
    } while (choice != 0);
    
    return 0;
}