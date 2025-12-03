#include "npc.h"
#include "npc_types.h"
#include "observer.h"
#include "visitor.h"
#include "factory.h"
#include "dungeon_editor.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <algorithm>
#include <sstream>
#include <set>


std::ostream& operator<<(std::ostream& os, const NPC& npc) {
    os << npc.getType() << " '" << npc.getName() << "' at " << npc.getPosition();
    return os;
}


SlaveTrader::SlaveTrader(const std::string& name, const Position& pos) 
    : NPC(name, pos) {}

std::string SlaveTrader::getType() const { 
    return "SlaveTrader"; 
}

int SlaveTrader::getPower() const { 
    return 70; 
}

int SlaveTrader::getDefense() const { 
    return 30; 
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
    return 40; 
}

int Squirrel::getDefense() const { 
    return 20; 
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
    return 90; 
}

int Knight::getDefense() const { 
    return 60; 
}

bool Knight::canAttack(const NPC& other) const {
    return other.getType() == "SlaveTrader";
}

void Knight::accept(Visitor& visitor) {
    visitor.visit(*this);
}


void ConsoleObserver::update(const std::string& message) {
    std::cout << "[LOG] " << message << std::endl;
}

FileObserver::FileObserver() {
    logFile.open("log.txt", std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Error opening log file!" << std::endl;
    }
}

FileObserver::~FileObserver() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void FileObserver::update(const std::string& message) {
    if (logFile.is_open()) {
        logFile << "[LOG] " << message << std::endl;
    }
}

void BattleLogger::attach(Observer* observer) {
    observers.push_back(observer);
}

void BattleLogger::detach(Observer* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void BattleLogger::notify(const std::string& message) {
    for (Observer* observer : observers) {
        observer->update(message);
    }
}


Visitor::Visitor(BattleLogger& logger, double range, std::vector<std::shared_ptr<NPC>>& npcs)
    : logger(logger), battleRange(range), npcs(npcs) {}

void Visitor::battle(NPC& attacker, NPC& defender) {
    if (markedForRemoval.find(&attacker) != markedForRemoval.end() ||
        markedForRemoval.find(&defender) != markedForRemoval.end()) {
        return;
    }
    
    if (!attacker.canAttack(defender)) {
        return;
    }
    
    if (attacker.getPosition().distanceTo(defender.getPosition()) > battleRange) {
        return;
    }
    
    int attackerPower = attacker.getPower();
    int defenderDefense = defender.getDefense();
    
    std::string battleLog = attacker.getName() + " (" + attacker.getType() + 
                           ") attacks " + defender.getName() + " (" + defender.getType() + ")";
    logger.notify(battleLog);
    
    if (attackerPower > defenderDefense) {
        logger.notify(defender.getName() + " (" + defender.getType() + ") was killed!");
        markForRemoval(defender);
    }
    
    if (markedForRemoval.find(&attacker) == markedForRemoval.end() &&
        defender.canAttack(attacker) && defender.getPower() > attacker.getDefense()) {
        logger.notify(attacker.getName() + " (" + attacker.getType() + ") was killed in counter-attack!");
        markForRemoval(attacker);
    }
}

void Visitor::markForRemoval(NPC& npc) {
    markedForRemoval.insert(&npc);
}

BattleVisitor::BattleVisitor(BattleLogger& logger, double range, std::vector<std::shared_ptr<NPC>>& npcs)
    : Visitor(logger, range, npcs) {}

void BattleVisitor::visit(SlaveTrader& trader) {
    if (getMarkedForRemoval().find(&trader) != getMarkedForRemoval().end()) {
        return;
    }
    
    for (auto& npc : npcs) {
        if (npc.get() != &trader && 
            getMarkedForRemoval().find(npc.get()) == getMarkedForRemoval().end()) {
            battle(trader, *npc);
        }
    }
}

void BattleVisitor::visit(Squirrel& squirrel) {
    if (getMarkedForRemoval().find(&squirrel) != getMarkedForRemoval().end()) {
        return;
    }
    
    for (auto& npc : npcs) {
        if (npc.get() != &squirrel && 
            getMarkedForRemoval().find(npc.get()) == getMarkedForRemoval().end()) {
            battle(squirrel, *npc);
        }
    }
}

void BattleVisitor::visit(Knight& knight) {
    if (getMarkedForRemoval().find(&knight) != getMarkedForRemoval().end()) {
        return;
    }
    
    for (auto& npc : npcs) {
        if (npc.get() != &knight && 
            getMarkedForRemoval().find(npc.get()) == getMarkedForRemoval().end()) {
            battle(knight, *npc);
        }
    }
}


std::shared_ptr<NPC> NPCFactory::createNPC(const std::string& type, 
                                          const std::string& name, 
                                          const Position& pos) {
    if (type == "SlaveTrader") {
        return std::make_shared<SlaveTrader>(name, pos);
    } else if (type == "Squirrel") {
        return std::make_shared<Squirrel>(name, pos);
    } else if (type == "Knight") {
        return std::make_shared<Knight>(name, pos);
    }
    throw std::runtime_error("Unknown NPC type: " + type);
}

std::shared_ptr<NPC> NPCFactory::loadFromString(const std::string& data) {
    std::istringstream iss(data);
    std::string type, name;
    double x, y;
    
    if (!(iss >> type >> name >> x >> y)) {
        throw std::runtime_error("Invalid NPC data format");
    }
    
    if (x < 0 || x > 500 || y < 0 || y > 500) {
        throw std::runtime_error("Coordinates out of bounds (0-500)");
    }
    
    return createNPC(type, name, Position(x, y));
}


DungeonEditor::DungeonEditor() {
    consoleObserver = std::make_shared<ConsoleObserver>();
    fileObserver = std::make_shared<FileObserver>();
    logger.attach(consoleObserver.get());
    logger.attach(fileObserver.get());
}

void DungeonEditor::addNPC(const std::string& type, const std::string& name, double x, double y) {
    if (x < 0 || x > 500 || y < 0 || y > 500) {
        std::cout << "Error: Coordinates must be between 0 and 500" << std::endl;
        return;
    }
    
    try {
        auto npc = NPCFactory::createNPC(type, name, Position(x, y));
        npcs.push_back(npc);
        std::cout << "Added " << type << " '" << name << "' at (" << x << ", " << y << ")" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void DungeonEditor::printNPCs() const {
    std::cout << "\nNPC List" << std::endl;
    if (npcs.empty()) {
        std::cout << "No NPCs in the dungeon." << std::endl;
    } else {
        for (const auto& npc : npcs) {
            std::cout << *npc << std::endl;
        }
        std::cout << "Total: " << npcs.size() << " NPCs" << std::endl;
    }
}

void DungeonEditor::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Cannot open file " << filename << std::endl;
        return;
    }
    
    for (const auto& npc : npcs) {
        file << npc->getType() << " " << npc->getName() << " " 
             << npc->getPosition().x << " " << npc->getPosition().y << std::endl;
    }
    
    file.close();
    std::cout << "Saved " << npcs.size() << " NPCs to " << filename << std::endl;
}

void DungeonEditor::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Cannot open file " << filename << std::endl;
        return;
    }
    
    npcs.clear();
    std::string line;
    int loaded = 0;
    
    while (std::getline(file, line)) {
        if (!line.empty()) {
            try {
                auto npc = NPCFactory::loadFromString(line);
                npcs.push_back(npc);
                loaded++;
            } catch (const std::exception& e) {
                std::cout << "Warning: " << e.what() << " in line: " << line << std::endl;
            }
        }
    }
    
    file.close();
    std::cout << "Loaded " << loaded << " NPCs from " << filename << std::endl;
}

void DungeonEditor::removeDeadNPCs(const std::set<NPC*>& deadNPCs) {
    if (deadNPCs.empty()) {
        return;
    }
    
    auto new_end = std::remove_if(npcs.begin(), npcs.end(),
        [&deadNPCs](const std::shared_ptr<NPC>& npc) {
            return deadNPCs.find(npc.get()) != deadNPCs.end();
        });
    
    size_t removed_count = std::distance(new_end, npcs.end());
    npcs.erase(new_end, npcs.end());
    
    if (removed_count > 0) {
        std::cout << "Removed " << removed_count << " dead NPCs from the dungeon." << std::endl;
    }
}

void DungeonEditor::startBattle(double range) {
    std::cout << "\nBattle mode (range: " << range << "m)" << std::endl;
    logger.notify("Battle started");
    
    size_t initialCount = npcs.size();
    
    BattleVisitor visitor(logger, range, npcs);
    
    for (auto& npc : npcs) {
        if (visitor.getMarkedForRemoval().find(npc.get()) == visitor.getMarkedForRemoval().end()) {
            npc->accept(visitor);
        }
    }
    
    removeDeadNPCs(visitor.getMarkedForRemoval());
    
    logger.notify("Battle ended");
    
    size_t survivors = npcs.size();
    size_t killed = initialCount - survivors;
    
    std::cout << "Battle completed." << std::endl;
    std::cout << "Initial NPCs: " << initialCount << std::endl;
    std::cout << "Killed: " << killed << std::endl;
    std::cout << "Survivors: " << survivors << std::endl;
    std::cout << "Check log.txt for details." << std::endl;
}

void DungeonEditor::createDemoNPCs() {
    addNPC("SlaveTrader", "Gorak", 100, 100);
    addNPC("Squirrel", "Nutty", 150, 120);
    addNPC("Squirrel", "Fluffy", 160, 130);
    addNPC("Knight", "Arthur", 200, 200);
    addNPC("SlaveTrader", "Boris", 250, 250);
    addNPC("Squirrel", "Chip", 50, 50);
    addNPC("Knight", "Lancelot", 300, 300);
}