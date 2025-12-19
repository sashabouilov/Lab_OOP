#include "game.h"
#include "game_constants.h"
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>

Game::Game() : gameRunning(false), gameTime(0) {
    editor.initializeRandomNPCs(GameConstants::INITIAL_NPC_COUNT);
}

Game::~Game() {
    stop();
}

void Game::movementWorker() {
    while (gameRunning) {
        processMovement();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Game::battleWorker() {
    while (gameRunning) {
        processBattles();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Game::mainWorker() {
    auto startTime = std::chrono::steady_clock::now();
    
    while (gameRunning) {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        gameTime = elapsed;
        
        if (elapsed >= GameConstants::GAME_DURATION_SECONDS) {
            gameRunning = false;
            break;
        }
        
        printMap();
        std::this_thread::sleep_for(std::chrono::milliseconds(GameConstants::PRINT_INTERVAL_MS));
    }
    
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "\nGAME OVER" << std::endl;
        std::cout << "Game duration: " << GameConstants::GAME_DURATION_SECONDS << " seconds" << std::endl;
        
        auto aliveNPCs = editor.getAliveNPCs();
        std::cout << "\nSurvivors (" << aliveNPCs.size() << "):" << std::endl;
        
        for (const auto& npc : aliveNPCs) {
            std::cout << "  " << *npc << std::endl;
        }
        
        std::cout << "\nTotal NPCs at start: " << GameConstants::INITIAL_NPC_COUNT << std::endl;
        std::cout << "Survivors: " << aliveNPCs.size() << std::endl;
        std::cout << "Killed: " << (GameConstants::INITIAL_NPC_COUNT - aliveNPCs.size()) << std::endl;
    }
}

void Game::processMovement() {
    auto npcs = editor.getNPCs();
    
    for (auto& npc : npcs) {
        if (npc->getIsAlive()) {
            npc->moveRandom();
        }
    }
    
    for (size_t i = 0; i < npcs.size(); ++i) {
        if (!npcs[i]->getIsAlive()) continue;
        
        for (size_t j = i + 1; j < npcs.size(); ++j) {
            if (!npcs[j]->getIsAlive()) continue;
            
            if (checkBattleCondition(*npcs[i], *npcs[j])) {
                std::lock_guard<std::mutex> lock(battleQueueMutex);
                battleQueue.emplace_back(npcs[i], npcs[j]);
            }
        }
    }
}

bool Game::checkBattleCondition(NPC& npc1, NPC& npc2) {
    double distance = npc1.getPosition().distanceTo(npc2.getPosition());
    
    if (distance <= npc1.getAttackRange() && npc1.canAttack(npc2)) {
        return true;
    }
    
    if (distance <= npc2.getAttackRange() && npc2.canAttack(npc1)) {
        return true;
    }
    
    return false;
}

void Game::processBattles() {
    std::vector<std::pair<std::shared_ptr<NPC>, std::shared_ptr<NPC>>> currentBattles;
    
    {
        std::lock_guard<std::mutex> lock(battleQueueMutex);
        currentBattles.swap(battleQueue);
    }
    
    for (auto& battle : currentBattles) {
        if (battle.first->getIsAlive() && battle.second->getIsAlive()) {
            if (battle.first->canAttack(*battle.second)) {
                resolveBattle(*battle.first, *battle.second);
            } else if (battle.second->canAttack(*battle.first)) {
                resolveBattle(*battle.second, *battle.first);
            }
        }
    }
}

void Game::resolveBattle(NPC& attacker, NPC& defender) {
    if (!attacker.getIsAlive() || !defender.getIsAlive()) return;
    
    int attackPower = attacker.getPower();
    int defensePower = defender.getDefense();
    
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "BATTLE: " << attacker.getName() << " (" << attacker.getType() 
                  << ") attacks " << defender.getName() << " (" << defender.getType() << ")" << std::endl;
        std::cout << "  Attack: " << attackPower << " vs Defense: " << defensePower << std::endl;
    }
    
    if (attackPower > defensePower) {
        defender.setIsAlive(false);
        
        {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << "  " << defender.getName() << " was killed!" << std::endl;
        }
        
        editor.getLogger().notify(defender.getName() + " was killed by " + attacker.getName());
    }
}

void Game::printMap() {
    std::lock_guard<std::mutex> lock(coutMutex);
    
    std::cout << "\nMAP (Time: " << gameTime << "/" 
              << GameConstants::GAME_DURATION_SECONDS << "s)" << std::endl;
    
    const int gridSize = 20;
    std::vector<std::vector<char>> grid(gridSize, std::vector<char>(gridSize, '.'));
    
    auto npcs = editor.getNPCs();
    
    for (const auto& npc : npcs) {
        if (npc->getIsAlive()) {
            Position pos = npc->getPosition();
            int x = static_cast<int>(pos.x * gridSize / GameConstants::MAP_WIDTH);
            int y = static_cast<int>(pos.y * gridSize / GameConstants::MAP_HEIGHT);
            
            x = std::max(0, std::min(x, gridSize - 1));
            y = std::max(0, std::min(y, gridSize - 1));
            
            char symbol = '?';
            std::string type = npc->getType();
            if (type == "SlaveTrader") symbol = 'S';
            else if (type == "Squirrel") symbol = 'Q';
            else if (type == "Knight") symbol = 'K';
            
            grid[y][x] = symbol;
        }
    }
    
    for (int y = 0; y < gridSize; ++y) {
        std::cout << "  ";
        for (int x = 0; x < gridSize; ++x) {
            std::cout << grid[y][x] << ' ';
        }
        std::cout << std::endl;
    }
    
    int aliveCount = 0;
    for (const auto& npc : npcs) {
        if (npc->getIsAlive()) aliveCount++;
    }
    
    std::cout << "\nAlive NPCs: " << aliveCount;
    std::cout << " | Game time: " << gameTime << "s";
    std::cout << std::endl;
}

void Game::initializeRandomNPCs(int count) {
    editor.initializeRandomNPCs(count);
}

void Game::run() {
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "\nSTARTING MULTITHREADED GAME" << std::endl;
        std::cout << "Map size: " << GameConstants::MAP_WIDTH << "x" << GameConstants::MAP_HEIGHT << std::endl;
        std::cout << "Game duration: " << GameConstants::GAME_DURATION_SECONDS << " seconds" << std::endl;
        std::cout << "Initial NPCs: " << GameConstants::INITIAL_NPC_COUNT << std::endl;
        std::cout << "\nStarting in 3 seconds..." << std::endl;
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    gameRunning = true;
    gameTime = 0;
    
    movementThread = std::thread(&Game::movementWorker, this);
    battleThread = std::thread(&Game::battleWorker, this);
    mainThread = std::thread(&Game::mainWorker, this);
    
    mainThread.join();
    gameRunning = false;
    
    movementThread.join();
    battleThread.join();
    
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "\nGame finished!" << std::endl;
    }
}

void Game::stop() {
    gameRunning = false;
    
    if (movementThread.joinable()) movementThread.join();
    if (battleThread.joinable()) battleThread.join();
    if (mainThread.joinable()) mainThread.join();
}

void Game::printMenu() {
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "\nMULTITHREADED NPC GAME" << std::endl;
    std::cout << "1. Starting game" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choice: ";
}