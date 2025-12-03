#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::string& message) = 0;
};

class ConsoleObserver : public Observer {
public:
    void update(const std::string& message) override;
};

class FileObserver : public Observer {
    std::ofstream logFile;
    
public:
    FileObserver();
    ~FileObserver();
    void update(const std::string& message) override;
};

class BattleLogger {
    std::vector<Observer*> observers;
    
public:
    void attach(Observer* observer);
    void detach(Observer* observer);
    void notify(const std::string& message);
};

#endif