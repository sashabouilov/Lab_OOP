#ifndef NPC_H
#define NPC_H

#include <string>
#include <memory>
#include "position.h"

class Visitor;

class NPC {
protected:
    std::string name;
    Position position;
    
public:
    NPC(const std::string& name, const Position& pos) : name(name), position(pos) {}
    virtual ~NPC() = default;
    
    virtual std::string getType() const = 0;
    virtual int getPower() const = 0;
    virtual int getDefense() const = 0;
    
    const std::string& getName() const { return name; }
    const Position& getPosition() const { return position; }
    
    virtual bool canAttack(const NPC& other) const = 0;
    virtual void accept(Visitor& visitor) = 0;
    
    friend std::ostream& operator<<(std::ostream& os, const NPC& npc);
};

#endif