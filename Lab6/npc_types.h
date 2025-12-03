#ifndef NPC_TYPES_H
#define NPC_TYPES_H

#include "npc.h"

class SlaveTrader;
class Squirrel;
class Knight;

class SlaveTrader : public NPC {
public:
    SlaveTrader(const std::string& name, const Position& pos);
    std::string getType() const override;
    int getPower() const override;
    int getDefense() const override;
    bool canAttack(const NPC& other) const override;
    void accept(Visitor& visitor) override;
};

class Squirrel : public NPC {
public:
    Squirrel(const std::string& name, const Position& pos);
    std::string getType() const override;
    int getPower() const override;
    int getDefense() const override;
    bool canAttack(const NPC& other) const override;
    void accept(Visitor& visitor) override;
};

class Knight : public NPC {
public:
    Knight(const std::string& name, const Position& pos);
    std::string getType() const override;
    int getPower() const override;
    int getDefense() const override;
    bool canAttack(const NPC& other) const override;
    void accept(Visitor& visitor) override;
};

#endif