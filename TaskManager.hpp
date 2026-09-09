#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

#include <vector>
#include <algorithm>
#include <iostream>
#include "Task.hpp"

class TaskManager {
private:
    std::vector<Task> tasks;
    int nextId = 1;

    std::vector<Task> getSortedTasks() const {
        std::vector<Task> sorted = tasks;
        std::sort(sorted.begin(), sorted.end(), [](const Task& a, const Task& b) {
            if (a.getPriority() != b.getPriority()) {
                return static_cast<int>(a.getPriority()) > static_cast<int>(b.getPriority());
            }
            return a.getId() < b.getId();
        });
        return sorted;
    }

public:
    const std::vector<Task>& getTasks() const {
        return tasks;
    }

    void addTask(const std::string& title, Priority priority = Priority::Medium, int parentId = 0, const std::vector<std::string>& tags = {}) {
        tasks.emplace_back(nextId++, title, priority, parentId, tags);
    }

    void loadTask(int id, const std::string& title, bool completed, Priority priority, int parentId = 0, const std::vector<std::string>& tags = {}) {
        tasks.emplace_back(id, title, priority, parentId, tags);
        if (completed) {
            tasks.back().markAsCompleted();
        }
        if (id >= nextId) {
            nextId = id + 1;
        }
    }

    void printProgressBar() const {
        if (tasks.empty()) return;

        int total = tasks.size();
        int completed = 0;
        for (const auto& task : tasks) {
            if (task.isCompleted()) completed++;
        }

        int percentage = (completed * 100) / total;
        int barWidth = 15;
        int filledWidth = (completed * barWidth) / total;

        std::cout << Color::GRAY << "Progress: [" << Color::RESET;
        for (int i = 0; i < barWidth; ++i) {
            if (i < filledWidth) std::cout << Color::GREEN << "█" << Color::RESET;
            else std::cout << Color::GRAY << "░" << Color::RESET;
        }
        std::cout << Color::GRAY << "] " << percentage << "% (" 
                  << completed << "/" << total << " completed)\n\n" << Color::RESET;
    }

    void listAllTasks() const {
        if (tasks.empty()) {
            std::cout << Color::GRAY << "No tasks found.\n" << Color::RESET;
            return;
        }
        printProgressBar();
        printTaskTree(false, false, Priority::Low, "");
    }

    void listTasksByStatus(bool showCompleted) const {
        if (tasks.empty()) {
            std::cout << Color::GREEN << "No tasks found! All caught up! 🎉\n" << Color::RESET;
            return;
        }
        printProgressBar();
        printTaskTree(true, showCompleted, Priority::Low, "");
    }

    void listTasksByPriority(Priority priority) const {
        printTaskTree(false, false, priority, "", true);
    }

    void listTasksByTag(const std::string& tag) const {
        printTaskTree(false, false, Priority::Low, tag, false, true);
    }

    void printTaskTree(bool filterStatus, bool statusValue, Priority priorityFilter = Priority::Low, const std::string& tagFilter = "", bool usePriority = false, bool useTag = false) const {
        auto sorted = getSortedTasks();
        bool found = false;

        // Imprime primeiro as tarefas principais
        for (const auto& task : sorted) {
            if (task.getParentId() != 0) continue; // Pula subtarefas na raiz

            bool matchStatus = !filterStatus || (task.isCompleted() == statusValue);
            bool matchPriority = !usePriority || (task.getPriority() == priorityFilter);
            bool matchTag = !useTag || task.hasTag(tagFilter);

            if (matchStatus && matchPriority && matchTag) {
                task.print(false);
                found = true;

                // Imprime as subtarefas vinculadas
                for (const auto& sub : sorted) {
                    if (sub.getParentId() == task.getId()) {
                        sub.print(true);
                    }
                }
            }
        }

        if (!found) {
            std::cout << Color::GRAY << "No matching tasks found.\n" << Color::RESET;
        }
    }

    void printStats() const {
        if (tasks.empty()) {
            std::cout << Color::GRAY << "No tasks available to generate statistics.\n" << Color::RESET;
            return;
        }

        int total = tasks.size();
        int completed = 0;
        int pending = 0;
        int highPrio = 0;
        int medPrio = 0;
        int lowPrio = 0;

        for (const auto& t : tasks) {
            if (t.isCompleted()) completed++;
            else pending++;

            if (t.getPriority() == Priority::High) highPrio++;
            else if (t.getPriority() == Priority::Medium) medPrio++;
            else if (t.getPriority() == Priority::Low) lowPrio++;
        }

        int rate = (completed * 100) / total;

        std::cout << Color::BOLD << "=== TASK STATISTICS ===\n\n" << Color::RESET
                  << " Total Tasks:      " << Color::CYAN << total << Color::RESET << "\n"
                  << " Completed:        " << Color::GREEN << completed << Color::RESET << " (" << rate << "%)\n"
                  << " Pending:          " << Color::YELLOW << pending << Color::RESET << "\n\n"
                  << Color::BOLD << "Priority Breakdown (Pending):\n" << Color::RESET
                  << "  High:            " << Color::RED << highPrio << Color::RESET << "\n"
                  << "  Medium:          " << Color::YELLOW << medPrio << Color::RESET << "\n"
                  << "  Low:             " << Color::BLUE << lowPrio << Color::RESET << "\n";
    }

    void searchTasks(const std::string& query) const {
        std::string lowerQuery = query;
        std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

        bool found = false;
        auto sortedTasks = getSortedTasks();

        for (const auto& task : sortedTasks) {
            std::string title = task.getTitle();
            std::transform(title.begin(), title.end(), title.begin(), ::tolower);

            if (title.find(lowerQuery) != std::string::npos) {
                task.print(task.getParentId() != 0);
                found = true;
            }
        }

        if (!found) {
            std::cout << Color::YELLOW << "No tasks matching \"" << query << "\".\n" << Color::RESET;
        }
    }

    bool editTask(int id, const std::string& newTitle, Priority newPriority, const std::vector<std::string>& newTags, bool updateTitle, bool updatePriority, bool updateTags) {
        auto it = std::find_if(tasks.begin(), tasks.end(), [id](const Task& t) {
            return t.getId() == id;
        });

        if (it != tasks.end()) {
            if (updateTitle) it->setTitle(newTitle);
            if (updatePriority) it->setPriority(newPriority);
            if (updateTags) it->setTags(newTags);
            return true;
        }
        return false;
    }

    bool markTaskCompleted(int id) {
        auto it = std::find_if(tasks.begin(), tasks.end(), [id](const Task& t) {
            return t.getId() == id;
        });

        if (it != tasks.end()) {
            it->markAsCompleted();
            return true;
        }
        return false;
    }

    bool deleteTask(int id) {
        auto it = std::find_if(tasks.begin(), tasks.end(), [id](const Task& t) {
            return t.getId() == id;
        });

        if (it != tasks.end()) {
            tasks.erase(it);
            return true;
        }
        return false;
    }

    int clearCompletedTasks() {
        int originalSize = tasks.size();
        tasks.erase(
            std::remove_if(tasks.begin(), tasks.end(), [](const Task& t) {
                return t.isCompleted();
            }),
            tasks.end()
        );
        return originalSize - tasks.size();
    }
};

#endif