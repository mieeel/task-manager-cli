#ifndef TASK_HPP
#define TASK_HPP

#include <string>
#include <iostream>
#include <iomanip>
#include "Colors.hpp"

enum class TaskStatus { Pending, Completed };

class Task {
private:
    int id;
    std::string title;
    TaskStatus status;

public:
    Task(int id, const std::string& title) 
        : id(id), title(title), status(TaskStatus::Pending) {}

    int getId() const { return id; }
    std::string getTitle() const { return title; }
    bool isCompleted() const { return status == TaskStatus::Completed; }

    void markAsCompleted() { 
        status = TaskStatus::Completed; 
    }

    void print() const {
        if (isCompleted()) {
            std::cout << Color::GREEN << "[X] " << Color::RESET
                      << Color::GRAY << std::setw(3) << std::setfill('0') << id << ". "
                      << title << Color::RESET << "\n";
        } else {
            std::cout << Color::YELLOW << "[ ] " << Color::RESET
                      << Color::BOLD << std::setw(3) << std::setfill('0') << id << ". " << Color::RESET
                      << Color::CYAN << title << Color::RESET << "\n";
        }
    }
};

#endif