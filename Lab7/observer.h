#ifndef OBSERVER_H
#define OBSERVER_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <ctime>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::string& message) = 0;
};

class Subject {
private:
    std::vector<std::shared_ptr<Observer>> observers;
    
public:
    void attach(std::shared_ptr<Observer> observer) {
        observers.push_back(observer);
    }
    
    void notify(const std::string& message) {
        for (auto& observer : observers) {
            observer->update(message);
        }
    }
};

class ConsoleObserver : public Observer {
public:
    void update(const std::string& message) override {
        std::cout << "[LOG] " << message << std::endl;
    }
};

class FileObserver : public Observer {
private:
    std::ofstream logFile;
    
public:
    FileObserver(const std::string& filename) {
        logFile.open(filename, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Error: Cannot open log file " << filename << std::endl;
        }
    }
    
    ~FileObserver() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    
    void update(const std::string& message) override {
        if (logFile.is_open()) {
            std::time_t now = std::time(nullptr);
            std::tm* timeinfo = std::localtime(&now);
            char timeStr[100];
            std::strftime(timeStr, sizeof(timeStr), "[%Y-%m-%d %H:%M:%S] ", timeinfo);
            
            logFile << timeStr << message << std::endl;
        }
    }
};

class BattleLogger : public Subject {
public:
    BattleLogger() = default;
};

#endif