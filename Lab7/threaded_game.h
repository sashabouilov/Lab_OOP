#ifndef THREADED_GAME_H
#define THREADED_GAME_H

#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "npc.h"
#include "observer.h"

class ThreadedGame {
private:
    std::vector<std::shared_ptr<NPC>> npcs;
    BattleLogger logger;
    std::shared_ptr<ConsoleObserver> consoleObserver;
    
    std::atomic<bool> gameRunning;
    std::atomic<int> gameTime;
    
    std::thread movementThread;
    std::thread battleThread;
    std::thread mainThread;
    
    std::mutex npcsMutex;
    std::mutex coutMutex;
    std::condition_variable cv;
    
    std::vector<std::pair<std::shared_ptr<NPC>, std::shared_ptr<NPC>>> battleQueue;
    std::mutex battleQueueMutex;
    
    void movementWorker();
    void battleWorker();
    void mainWorker();
    
    void processMovement();
    void processBattles();
    void printMap();
    void removeDeadNPCs();
    
    bool checkBattleCondition(NPC& npc1, NPC& npc2);
    void resolveBattle(NPC& attacker, NPC& defender);
    
public:
    ThreadedGame();
    ~ThreadedGame();
    
    void initializeRandomNPCs(int count);
    void run();
    void stop();
    void printSurvivors() const;
};

#endif