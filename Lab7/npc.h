#ifndef NPC_H
#define NPC_H

#include <string>
#include <memory>
#include <mutex>
#include <random>
#include "position.h"
#include "game_constants.h"

class Visitor;

class NPC {
protected:
    std::string name;
    Position position;
    bool isAlive;
    mutable std::mutex npcMutex;
    
    static std::random_device rd;
    static std::mt19937 gen;
    
public:
    NPC(const std::string& name, const Position& pos);
    virtual ~NPC() = default;
    
    virtual std::string getType() const = 0;
    virtual int getPower() const = 0;
    virtual int getDefense() const = 0;
    virtual int getMoveDistance() const = 0;
    virtual int getAttackRange() const = 0;
    
    const std::string& getName() const { return name; }
    Position getPosition() const;
    bool getIsAlive() const;
    
    void setPosition(const Position& pos);
    void setIsAlive(bool alive);
    
    virtual bool canAttack(const NPC& other) const = 0;
    virtual void accept(Visitor& visitor) = 0;
    
    void moveRandom();
    int rollDice() const;
    
    friend std::ostream& operator<<(std::ostream& os, const NPC& npc);
};

#endif