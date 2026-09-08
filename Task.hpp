#ifndef TASK_HPP
#define TASK_HPP

#include <string>
#include <iostream>
#include <iomanip>

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
        std::cout << "[" << (isCompleted() ? "X" : " ") << "] "
                  << std::setw(3) << std::setfill('0') << id << ". " 
                  << title << "\n";
    }
};

#endif