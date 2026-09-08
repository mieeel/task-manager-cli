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

    void addTask(const std::string& title, Priority priority = Priority::Medium) {
        tasks.emplace_back(nextId++, title, priority);
    }

    void loadTask(int id, const std::string& title, bool completed, Priority priority) {
        tasks.emplace_back(id, title, priority);
        if (completed) {
            tasks.back().markAsCompleted();
        }
        if (id >= nextId) {
            nextId = id + 1;
        }
    }

    void listAllTasks() const {
        if (tasks.empty()) {
            std::cout << Color::GRAY << "No tasks found.\n" << Color::RESET;
            return;
        }
        auto sortedTasks = getSortedTasks();
        for (const auto& task : sortedTasks) {
            task.print();
        }
    }

    void listTasksByStatus(bool showCompleted) const {
        bool foundAny = false;
        auto sortedTasks = getSortedTasks();

        for (const auto& task : sortedTasks) {
            if (task.isCompleted() == showCompleted) {
                task.print();
                foundAny = true;
            }
        }

        if (!foundAny) {
            if (showCompleted) {
                std::cout << Color::GRAY << "No completed tasks yet.\n" << Color::RESET;
            } else {
                std::cout << Color::GREEN << "No pending tasks! All caught up! 🎉\n" << Color::RESET;
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