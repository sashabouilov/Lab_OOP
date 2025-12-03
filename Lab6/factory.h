#ifndef FACTORY_H
#define FACTORY_H

#include <memory>
#include <string>
#include <stdexcept>
#include <sstream>
#include "npc_types.h"

class NPCFactory {
public:
    static std::shared_ptr<NPC> createNPC(const std::string& type, 
                                          const std::string& name, 
                                          const Position& pos);
    
    static std::shared_ptr<NPC> loadFromString(const std::string& data);
};

#endif