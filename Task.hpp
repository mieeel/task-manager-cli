#ifndef TASK_HPP
#define TASK_HPP

#include <string>
#include <iostream>
#include <iomanip>
#include "Colors.hpp"

enum class TaskStatus { Pending, Completed };
enum class Priority { Low = 1, Medium = 2, High = 3 };

class Task {
private:
    int id;
    std::string title;
    TaskStatus status;
    Priority priority;

public:
    Task(int id, const std::string& title, Priority priority = Priority::Medium) 
        : id(id), title(title), status(TaskStatus::Pending), priority(priority) {}

    int getId() const { return id; }
    std::string getTitle() const { return title; }
    bool isCompleted() const { return status == TaskStatus::Completed; }
    Priority getPriority() const { return priority; }

    void markAsCompleted() { 
        status = TaskStatus::Completed; 
    }

    std::string getColoredPriority() const {
        switch (priority) {
            case Priority::High:   
                return Color::BOLD + Color::RED + "[HIGH]" + Color::RESET;
            case Priority::Medium: 
                return Color::YELLOW + "[MED ]" + Color::RESET;
            case Priority::Low:    
                return Color::BLUE + "[LOW ]" + Color::RESET;
        }
        return Color::YELLOW + "[MED ]" + Color::RESET;
    }

    void print() const {
        if (isCompleted()) {
            std::cout << Color::GREEN << "[X] " << Color::RESET
                      << Color::GRAY << std::setw(3) << std::setfill('0') << id << ". "
                      << "[DONE] " << title << Color::RESET << "\n";
        } else {
            std::cout << Color::YELLOW << "[ ] " << Color::RESET
                      << Color::BOLD << std::setw(3) << std::setfill('0') << id << ". " << Color::RESET
                      << getColoredPriority() << " "
                      << Color::CYAN << title << Color::RESET << "\n";
        }
    }
};

#endif