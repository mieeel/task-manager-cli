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

public:
    const std::vector<Task>& getTasks() const {
        return tasks;
    }

    void addTask(const std::string& title) {
        tasks.emplace_back(nextId++, title);
    }

    void loadTask(int id, const std::string& title, bool completed) {
        tasks.emplace_back(id, title);
        if (completed) {
            tasks.back().markAsCompleted();
        }
        if (id >= nextId) {
            nextId = id + 1;
        }
    }

    // Lista apenas as PENDENTES ou apenas as CONCLUÍDAS
    void listTasksByStatus(bool showCompleted) const {
        bool foundAny = false;
        for (const auto& task : tasks) {
            if (task.isCompleted() == showCompleted) {
                task.print();
                foundAny = true;
            }
        }

        if (!foundAny) {
            if (showCompleted) {
                std::cout << "Nenhuma tarefa concluida por enquanto.\n";
            } else {
                std::cout << "Nenhuma tarefa pendente! Tudo em dia! 🎉\n";
            }
        }
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
};

#endif