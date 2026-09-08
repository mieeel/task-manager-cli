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
        std::ofstream file(filename, std::ios::trunc); // Sobrescreve o arquivo com a lista atual
        if (!file.is_open()) return;

        for (const auto& task : manager.getTasks()) {
            file << task.getId() << "|" 
                 << (task.isCompleted() ? "1" : "0") << "|" 
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
            std::string idStr, statusStr, title;

            if (std::getline(ss, idStr, '|') &&
                std::getline(ss, statusStr, '|') &&
                std::getline(ss, title)) {
                
                int id = std::stoi(idStr);
                bool completed = (statusStr == "1");

                // Recria a tarefa mantendo o estado salvo
                manager.loadTask(id, title, completed);
            }
        }
    }
};

#endif