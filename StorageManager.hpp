#ifndef STORAGE_MANAGER_HPP
#define STORAGE_MANAGER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "TaskManager.hpp"

class StorageManager {
private:
    std::string filename;

public:
    explicit StorageManager(const std::string& filename) : filename(filename) {}

    void save(const TaskManager& manager) const {
        std::ofstream file(filename, std::ios::trunc);
        if (!file.is_open()) return;

        for (const auto& task : manager.getTasks()) {
            file << task.getId() << "|" 
                 << (task.isCompleted() ? "1" : "0") << "|" 
                 << static_cast<int>(task.getPriority()) << "|"
                 << task.getTitle() << "\n";
        }
    }

    void load(TaskManager& manager) const {
        std::ifstream file(filename);
        if (!file.is_open()) return;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string idStr, statusStr, prioStr, title;

            // Suporta o formato novo com prioridade (ID|Status|Priority|Title)
            if (std::getline(ss, idStr, '|') &&
                std::getline(ss, statusStr, '|') &&
                std::getline(ss, prioStr, '|') &&
                std::getline(ss, title)) {
                
                int id = std::stoi(idStr);
                bool completed = (statusStr == "1");
                Priority priority = static_cast<Priority>(std::stoi(prioStr));

                manager.loadTask(id, title, completed, priority);
            }
        }
    }
};

#endif