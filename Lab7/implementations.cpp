#include "npc.h"
#include "npc_types.h"
#include "observer.h"
#include "visitor.h"
#include "factory.h"
#include "dungeon_editor.h"
#include "game.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <algorithm>
#include <sstream>
#include <set>
#include <random>
#include <chrono>

std::random_device NPC::rd;
std::mt19937 NPC::gen(NPC::rd());

NPC::NPC(const std::string& name, const Position& pos) 
    : name(name), position(pos), isAlive(true) {}

Position NPC::getPosition() const {
    std::lock_guard<std::mutex> lock(npcMutex);
    return position;
}

bool NPC::getIsAlive() const {
    std::lock_guard<std::mutex> lock(npcMutex);
    return isAlive;
}

void NPC::setPosition(const Position& pos) {
    std::lock_guard<std::mutex> lock(npcMutex);
    position = pos;
}

void NPC::setIsAlive(bool alive) {
    std::lock_guard<std::mutex> lock(npcMutex);
    isAlive = alive;
}

void NPC::moveRandom() {
    if (!getIsAlive()) return;
    
    std::uniform_real_distribution<> angleDist(0, 2 * 3.14159);
    std::uniform_real_distribution<> distDist(0, 1);
    
    double angle = angleDist(gen);
    double distance = distDist(gen) * getMoveDistance();
    
    Position currentPos = getPosition();
    double newX = currentPos.x + distance * cos(angle);
    double newY = currentPos.y + distance * sin(angle);
    
    newX = std::max(0.0, std::min(newX, (double)GameConstants::MAP_WIDTH));
    newY = std::max(0.0, std::min(newY, (double)GameConstants::MAP_HEIGHT));
    
    setPosition(Position(newX, newY));
}

int NPC::rollDice() const {
    std::uniform_int_distribution<> dist(1, 6);
    return dist(gen);
}

std::ostream& operator<<(std::ostream& os, const NPC& npc) {
    std::lock_guard<std::mutex> lock(npc.npcMutex);
    os << npc.getType() << " '" << npc.name << "' at " << npc.position 
       << " (" << (npc.isAlive ? "Alive" : "Dead") << ")";
    return os;
}

SlaveTrader::SlaveTrader(const std::string& name, const Position& pos) 
    : NPC(name, pos) {}

std::string SlaveTrader::getType() const { 
    return "SlaveTrader"; 
}

int SlaveTrader::getPower() const { 
    return GameConstants::NPC_STATS.at("SlaveTrader").basePower + rollDice() * 5;
}

int SlaveTrader::getDefense() const { 
    return GameConstants::NPC_STATS.at("SlaveTrader").baseDefense + rollDice() * 3;
}

int SlaveTrader::getMoveDistance() const {
    return GameConstants::NPC_STATS.at("SlaveTrader").moveDistance;
}

int SlaveTrader::getAttackRange() const {
    return GameConstants::NPC_STATS.at("SlaveTrader").attackRange;
}

bool SlaveTrader::canAttack(const NPC& other) const {
    return other.getType() == "Squirrel";
}

void SlaveTrader::accept(Visitor& visitor) {
    visitor.visit(*this);
}

Squirrel::Squirrel(const std::string& name, const Position& pos) 
    : NPC(name, pos) {}

std::string Squirrel::getType() const { 
    return "Squirrel"; 
}

int Squirrel::getPower() const { 
    return GameConstants::NPC_STATS.at("Squirrel").basePower + rollDice() * 5;
}

int Squirrel::getDefense() const { 
    return GameConstants::NPC_STATS.at("Squirrel").baseDefense + rollDice() * 3;
}

int Squirrel::getMoveDistance() const {
    return GameConstants::NPC_STATS.at("Squirrel").moveDistance;
}

int Squirrel::getAttackRange() const {
    return GameConstants::NPC_STATS.at("Squirrel").attackRange;
}

bool Squirrel::canAttack(const NPC& other) const {
    return other.getType() == "Squirrel";
}

void Squirrel::accept(Visitor& visitor) {
    visitor.visit(*this);
}

Knight::Knight(const std::string& name, const Position& pos) 
    : NPC(name, pos) {}

std::string Knight::getType() const { 
    return "Knight"; 
}

int Knight::getPower() const { 
    return GameConstants::NPC_STATS.at("Knight").basePower + rollDice() * 5;
}

int Knight::getDefense() const { 
    return GameConstants::NPC_STATS.at("Knight").baseDefense + rollDice() * 3;
}

int Knight::getMoveDistance() const {
    return GameConstants::NPC_STATS.at("Knight").moveDistance;
}

int Knight::getAttackRange() const {
    return GameConstants::NPC_STATS.at("Knight").attackRange;
}

bool Knight::canAttack(const NPC& other) const {
    return other.getType() == "SlaveTrader";
}

void Knight::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void DungeonEditor::initializeRandomNPCs(int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> posDist(0, GameConstants::MAP_WIDTH);
    std::uniform_int_distribution<> typeDist(0, 2);
    
    std::vector<std::string> types = {"SlaveTrader", "Squirrel", "Knight"};
    
    {
        std::unique_lock<std::shared_mutex> lock(npcsMutex);
        npcs.clear();
        
        for (int i = 0; i < count; ++i) {
            std::string type = types[typeDist(gen)];
            std::string name = type + "_" + std::to_string(i);
            Position pos(posDist(gen), posDist(gen));
            
            auto npc = NPCFactory::createNPC(type, name, pos);
            npcs.push_back(npc);
        }
    }
    
    std::cout << "Initialized " << count << " random NPCs" << std::endl;
}

std::vector<std::shared_ptr<NPC>> DungeonEditor::getNPCs() const {
    std::shared_lock<std::shared_mutex> lock(npcsMutex);
    return npcs;
}

size_t DungeonEditor::getNPCsCount() const {
    std::shared_lock<std::shared_mutex> lock(npcsMutex);
    return npcs.size();
}

void DungeonEditor::clearNPCs() {
    std::unique_lock<std::shared_mutex> lock(npcsMutex);
    npcs.clear();
}

std::vector<std::shared_ptr<NPC>> DungeonEditor::getAliveNPCs() const {
    std::shared_lock<std::shared_mutex> lock(npcsMutex);
    std::vector<std::shared_ptr<NPC>> aliveNPCs;
    
    for (const auto& npc : npcs) {
        if (npc->getIsAlive()) {
            aliveNPCs.push_back(npc);
        }
    }
    
    return aliveNPCs;
}

DungeonEditor::DungeonEditor() {
    consoleObserver = std::make_shared<ConsoleObserver>();
    fileObserver = std::make_shared<FileObserver>("battle_log.txt");
    
    logger.attach(consoleObserver);
    logger.attach(fileObserver);
    
    NPCFactory::initialize();
}

// Метод для добавления NPC
void DungeonEditor::addNPC(const std::string& type, const std::string& name, double x, double y) {
    if (x < 0 || x > GameConstants::MAP_WIDTH || y < 0 || y > GameConstants::MAP_HEIGHT) {
        std::cout << "Error: Coordinates must be between 0 and " 
                  << GameConstants::MAP_WIDTH << std::endl;
        return;
    }
    
    try {
        Position pos(x, y);
        auto npc = NPCFactory::createNPC(type, name, pos);
        
        std::unique_lock<std::shared_mutex> lock(npcsMutex);
        npcs.push_back(npc);
        
        std::cout << "Added " << type << " '" << name << "' at (" 
                  << x << ", " << y << ")" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        std::cout << "Available types: SlaveTrader, Squirrel, Knight" << std::endl;
    }
}

// Метод для печати всех NPC
void DungeonEditor::printNPCs() const {
    std::shared_lock<std::shared_mutex> lock(npcsMutex);
    
    if (npcs.empty()) {
        std::cout << "No NPCs in the dungeon." << std::endl;
        return;
    }
    
    std::cout << "\nCurrent NPCs (" << npcs.size() << "):" << std::endl;
    for (size_t i = 0; i < npcs.size(); ++i) {
        std::cout << i + 1 << ". " << *npcs[i] << std::endl;
    }
}

// Метод для сохранения в файл
void DungeonEditor::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Cannot open file " << filename << std::endl;
        return;
    }
    
    std::shared_lock<std::shared_mutex> lock(npcsMutex);
    
    for (const auto& npc : npcs) {
        if (npc->getIsAlive()) {
            NPCSerializer serializer(file);
            npc->accept(serializer);
            file << std::endl;
        }
    }
    
    file.close();
    std::cout << "Saved " << npcs.size() << " NPCs to " << filename << std::endl;
}

// Метод для загрузки из файла
void DungeonEditor::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Cannot open file " << filename << std::endl;
        return;
    }
    
    std::unique_lock<std::shared_mutex> lock(npcsMutex);
    npcs.clear();
    
    std::string line;
    int count = 0;
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type, name;
        double x, y;
        
        if (iss >> type >> name >> x >> y) {
            try {
                Position pos(x, y);
                auto npc = NPCFactory::createNPC(type, name, pos);
                npcs.push_back(npc);
                count++;
            }
            catch (const std::exception& e) {
                std::cout << "Warning: Skipping invalid NPC data: " << line << std::endl;
            }
        }
    }
    
    file.close();
    std::cout << "Loaded " << count << " NPCs from " << filename << std::endl;
}

// Метод для начала битвы
void DungeonEditor::startBattle(double range) {
    std::shared_lock<std::shared_mutex> lock(npcsMutex);
    std::set<NPC*> deadNPCs;
    
    for (size_t i = 0; i < npcs.size(); ++i) {
        if (!npcs[i]->getIsAlive()) continue;
        
        for (size_t j = i + 1; j < npcs.size(); ++j) {
            if (!npcs[j]->getIsAlive()) continue;
            
            double distance = npcs[i]->getPosition().distanceTo(npcs[j]->getPosition());
            if (distance <= range) {
                if (npcs[i]->canAttack(*npcs[j])) {
                    resolveBattle(*npcs[i], *npcs[j]);
                    if (!npcs[j]->getIsAlive()) {
                        deadNPCs.insert(npcs[j].get());
                    }
                }
                else if (npcs[j]->canAttack(*npcs[i])) {
                    resolveBattle(*npcs[j], *npcs[i]);
                    if (!npcs[i]->getIsAlive()) {
                        deadNPCs.insert(npcs[i].get());
                    }
                }
            }
        }
    }
    
    if (!deadNPCs.empty()) {
        removeDeadNPCs(deadNPCs);
    }
}

// Метод для создания демо NPC
void DungeonEditor::createDemoNPCs() {
    std::unique_lock<std::shared_mutex> lock(npcsMutex);
    
    // Очищаем существующих NPC
    npcs.clear();
    
    // Создаем демо NPC
    addNPC("Knight", "Arthur", 100, 100);
    addNPC("Knight", "Lancelot", 200, 200);
    addNPC("SlaveTrader", "Merchant", 150, 150);
    addNPC("Squirrel", "Chip", 120, 180);
    addNPC("Squirrel", "Dale", 180, 120);
    
    std::cout << "Created 5 demo NPCs." << std::endl;
}

// Приватный метод для удаления мертвых NPC
void DungeonEditor::removeDeadNPCs(const std::set<NPC*>& deadNPCs) {
    std::unique_lock<std::shared_mutex> lock(npcsMutex);
    
    auto new_end = std::remove_if(npcs.begin(), npcs.end(),
        [&deadNPCs](const std::shared_ptr<NPC>& npc) {
            return deadNPCs.find(npc.get()) != deadNPCs.end();
        });
    
    npcs.erase(new_end, npcs.end());
}

// Метод для разрешения битвы
void DungeonEditor::resolveBattle(NPC& attacker, NPC& defender) {
    if (!attacker.getIsAlive() || !defender.getIsAlive()) return;
    
    int attackPower = attacker.getPower();
    int defensePower = defender.getDefense();
    
    std::cout << "BATTLE: " << attacker.getName() << " (" << attacker.getType() 
              << ") attacks " << defender.getName() << " (" << defender.getType() << ")" << std::endl;
    std::cout << "  Attack: " << attackPower << " vs Defense: " << defensePower << std::endl;
    
    if (attackPower > defensePower) {
        defender.setIsAlive(false);
        std::cout << "  " << defender.getName() << " was killed!" << std::endl;
        logger.notify(defender.getName() + " was killed by " + attacker.getName());
    }
    else {
        std::cout << "  " << defender.getName() << " defended successfully!" << std::endl;
    }
}