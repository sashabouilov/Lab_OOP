#ifndef VISITOR_H
#define VISITOR_H

#include <vector>
#include <memory>
#include <string>
#include <set>
#include "observer.h"

class NPC;
class SlaveTrader;
class Squirrel;
class Knight;

class Visitor {
protected:
    BattleLogger& logger;
    double battleRange;
    std::vector<std::shared_ptr<NPC>>& npcs;
    std::set<NPC*> markedForRemoval;
    
public:
    Visitor(BattleLogger& logger, double range, std::vector<std::shared_ptr<NPC>>& npcs);
    virtual ~Visitor() = default;
    
    virtual void visit(SlaveTrader& trader) = 0;
    virtual void visit(Squirrel& squirrel) = 0;
    virtual void visit(Knight& knight) = 0;
    
    void battle(NPC& attacker, NPC& defender);
    void markForRemoval(NPC& npc);
    
    const std::set<NPC*>& getMarkedForRemoval() const { return markedForRemoval; }
    void clearMarkedForRemoval() { markedForRemoval.clear(); }
};

class BattleVisitor : public Visitor {
public:
    BattleVisitor(BattleLogger& logger, double range, std::vector<std::shared_ptr<NPC>>& npcs);
    
    void visit(SlaveTrader& trader) override;
    void visit(Squirrel& squirrel) override;
    void visit(Knight& knight) override;
};

#endif