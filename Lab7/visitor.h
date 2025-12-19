#ifndef VISITOR_H
#define VISITOR_H

class SlaveTrader;
class Squirrel;
class Knight;

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(SlaveTrader& npc) = 0;
    virtual void visit(Squirrel& npc) = 0;
    virtual void visit(Knight& npc) = 0;
};

class NPCSerializer : public Visitor {
private:
    std::ostream& os;
    
public:
    NPCSerializer(std::ostream& os) : os(os) {}
    
    void visit(SlaveTrader& npc) override {
        os << "SlaveTrader " << npc.getName() << " " 
           << npc.getPosition().x << " " << npc.getPosition().y;
    }
    
    void visit(Squirrel& npc) override {
        os << "Squirrel " << npc.getName() << " " 
           << npc.getPosition().x << " " << npc.getPosition().y;
    }
    
    void visit(Knight& npc) override {
        os << "Knight " << npc.getName() << " " 
           << npc.getPosition().x << " " << npc.getPosition().y;
    }
};

class NPCStatsVisitor : public Visitor {
private:
    std::ostream& os;
    
public:
    NPCStatsVisitor(std::ostream& os) : os(os) {}
    
    void visit(SlaveTrader& npc) override {
        os << "SlaveTrader '" << npc.getName() << "': "
           << "Power=" << npc.getPower() 
           << ", Defense=" << npc.getDefense()
           << ", Range=" << npc.getAttackRange();
    }
    
    void visit(Squirrel& npc) override {
        os << "Squirrel '" << npc.getName() << "': "
           << "Power=" << npc.getPower() 
           << ", Defense=" << npc.getDefense()
           << ", Range=" << npc.getAttackRange();
    }
    
    void visit(Knight& npc) override {
        os << "Knight '" << npc.getName() << "': "
           << "Power=" << npc.getPower() 
           << ", Defense=" << npc.getDefense()
           << ", Range=" << npc.getAttackRange();
    }
};

#endif