#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include <map>
#include <string>

namespace GameConstants {
    const int MAP_WIDTH = 500;
    const int MAP_HEIGHT = 500;
    const int GAME_DURATION_SECONDS = 30;
    const int INITIAL_NPC_COUNT = 50;
    const int PRINT_INTERVAL_MS = 1000;
    
    struct NPCStats {
        int moveDistance;
        int attackRange;
        int basePower;
        int baseDefense;
    };
    
    const std::map<std::string, NPCStats> NPC_STATS = {
        {"SlaveTrader", {10, 10, 70, 30}},
        {"Squirrel", {5, 5, 40, 20}},
        {"Knight", {30, 10, 90, 60}},
        {"Orc", {20, 10, 80, 40}},
        {"Druid", {10, 10, 60, 40}},
        {"Elf", {10, 50, 75, 35}},
        {"Dragon", {50, 30, 100, 80}},
        {"Bear", {5, 10, 85, 65}},
        {"Bandit", {10, 10, 65, 35}},
        {"Werewolf", {40, 5, 75, 50}},
        {"Princess", {1, 1, 10, 10}},
        {"Toad", {1, 10, 30, 20}},
        {"Pegasus", {30, 10, 75, 45}},
        {"Bittern", {50, 10, 65, 35}},
        {"Desman", {5, 20, 55, 35}},
        {"Bull", {30, 10, 80, 50}}
    };
}

#endif