#ifndef FACTORY_H
#define FACTORY_H

#include "npc.h"
#include "npc_types.h"
#include "position.h"
#include <memory>
#include <map>
#include <functional>

class NPCFactory {
private:
    using Creator = std::function<std::shared_ptr<NPC>(const std::string&, const Position&)>;
    static std::map<std::string, Creator> creators;
    
public:
    static void registerCreator(const std::string& type, Creator creator) {
        creators[type] = creator;
    }
    
    static std::shared_ptr<NPC> createNPC(const std::string& type, 
                                          const std::string& name, 
                                          const Position& pos) {
        auto it = creators.find(type);
        if (it != creators.end()) {
            return it->second(name, pos);
        }
        throw std::runtime_error("Unknown NPC type: " + type);
    }
    
    static void initialize() {
        if (creators.empty()) {
            registerCreator("SlaveTrader", [](const std::string& name, const Position& pos) {
                return std::make_shared<SlaveTrader>(name, pos);
            });
            
            registerCreator("Squirrel", [](const std::string& name, const Position& pos) {
                return std::make_shared<Squirrel>(name, pos);
            });
            
            registerCreator("Knight", [](const std::string& name, const Position& pos) {
                return std::make_shared<Knight>(name, pos);
            });
        }
    }
    
    static std::vector<std::string> getAvailableTypes() {
        std::vector<std::string> types;
        for (const auto& pair : creators) {
            types.push_back(pair.first);
        }
        return types;
    }
};

// Инициализация статического члена
std::map<std::string, NPCFactory::Creator> NPCFactory::creators;

#endif