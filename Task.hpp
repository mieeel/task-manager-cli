#ifndef TASK_HPP
#define TASK_HPP

#include <string>
#include <iostream>

enum class TaskStatus { Pending, Completed };

class Task {
private:
    int id;
    std::string title;
    TaskStatus status;

public:
    // Construtor principal (cria novas tarefas pendentes por padrão)
    Task(int id, const std::string& title) 
        : id(id), title(title), status(TaskStatus::Pending) {}

    // Getters
    int getId() const { return id; }
    std::string getTitle() const { return title; }
    bool isCompleted() const { return status == TaskStatus::Completed; }

    // Métodos para alterar estado
    void markAsCompleted() { 
        status = TaskStatus::Completed; 
    }

    // Exibição formatada no terminal
    void print() const {
        std::cout << "[" << (isCompleted() ? "X" : " ") << "] "
                  << id << ". " << title << "\n";
    }
};

#endif