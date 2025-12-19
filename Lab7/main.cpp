#include "game.h"
#include <iostream>

int main() {
    Game game;
    int choice;
    
    do {
        game.printMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                std::cout << "\nStarting game with added NPC..." << std::endl;
                
                Game newGame;
                newGame.initializeRandomNPCs(50);

                std::cout << "NPC added. Starting game..." << std::endl;
                newGame.run();
                break;
            }
            
            case 0:
                std::cout << "Exiting..." << std::endl;
                break;
                
            default:
                std::cout << "Invalid choice" << std::endl;
        }
        
    } while (choice != 0);
    
    return 0;
}