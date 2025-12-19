#ifndef GAME_H
#define GAME_H

#include <thread>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <condition_variable>
#include <vector>
#include <memory>
#include "dungeon_editor.h"

class Game {
private:
    DungeonEditor editor;
    
    std::atomic<bool> gameRunning;
    std::atomic<int> gameTime;
    
    std::thread movementThread;
    std::thread battleThread;
    std::thread mainThread;
    
    std::shared_mutex npcsMutex;
    std::mutex coutMutex;
    std::mutex battleQueueMutex;
    std::condition_variable cv;
    
    std::vector<std::pair<std::shared_ptr<NPC>, std::shared_ptr<NPC>>> battleQueue;
    
    void movementWorker();
    void battleWorker();
    void mainWorker();
    
    void processMovement();
    void processBattles();
    void printMap();
    
    bool checkBattleCondition(NPC& npc1, NPC& npc2);
    void resolveBattle(NPC& attacker, NPC& defender);
    
public:
    Game();
    ~Game();
    
    void initializeRandomNPCs(int count);
    void run();
    void stop();
    void printMenu();
};

#endif