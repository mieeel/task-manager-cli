#ifndef STORAGE_MANAGER_HPP
#define STORAGE_MANAGER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
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
            std::string tagsStr = "";
            for (size_t i = 0; i < task.getTags().size(); ++i) {
                tagsStr += task.getTags()[i];
                if (i + 1 < task.getTags().size()) tagsStr += ",";
            }

            file << task.getId() << "|" 
                 << (task.isCompleted() ? "1" : "0") << "|" 
                 << static_cast<int>(task.getPriority()) << "|"
                 << task.getParentId() << "|"
                 << tagsStr << "|"
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
            std::string idStr, statusStr, prioStr, parentStr, tagsRaw, title;

            if (std::getline(ss, idStr, '|') &&
                std::getline(ss, statusStr, '|') &&
                std::getline(ss, prioStr, '|')) {
                
                int id = std::stoi(idStr);
                bool completed = (statusStr == "1");
                Priority priority = static_cast<Priority>(std::stoi(prioStr));
                int parentId = 0;
                std::vector<std::string> tags;

                // Suporte a formatos legados e novos
                std::string remainder;
                if (std::getline(ss, remainder)) {
                    std::stringstream rss(remainder);
                    if (std::getline(rss, parentStr, '|') &&
                        std::getline(rss, tagsRaw, '|') &&
                        std::getline(rss, title)) {
                        
                        parentId = std::stoi(parentStr);
                        std::stringstream tss(tagsRaw);
                        std::string tag;
                        while (std::getline(tss, tag, ',')) {
                            if (!tag.empty()) tags.push_back(tag);
                        }
                    } else {
                        title = remainder; // Formato antigo sem parent/tags
                    }
                }

                manager.loadTask(id, title, completed, priority, parentId, tags);
            }
        }
    }
};

#endif