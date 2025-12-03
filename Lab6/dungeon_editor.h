#ifndef DUNGEON_EDITOR_H
#define DUNGEON_EDITOR_H

#include <vector>
#include <memory>
#include <string>
#include <set>
#include "npc.h"
#include "observer.h"


class DungeonEditor {
private:
    std::vector<std::shared_ptr<NPC>> npcs;
    BattleLogger logger;
    std::shared_ptr<ConsoleObserver> consoleObserver;
    std::shared_ptr<FileObserver> fileObserver;
    
    void removeDeadNPCs(const std::set<NPC*>& deadNPCs);
    
public:
    DungeonEditor();
    
    void addNPC(const std::string& type, const std::string& name, double x, double y);
    void printNPCs() const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void startBattle(double range);
    void createDemoNPCs();
};

#endif