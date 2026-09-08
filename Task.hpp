#include <iostream>
#include <string.h>

enum class TaskStatus {Pendente, Completa};

class Task{
    private:
    int id;
    std::string title;
    TaskStatus status;

    public:
    Task(int id, const std::string& title): id(id), title(title), status(TaskStatus::Pendente){}

    int getID() const {return id; }
    std::string getTitle() const {return title; }
    bool isCompleted() const {return status == TaskStatus::Completa; }

    void markAsCompleted() { status = TaskStatus::Completa; }

    void print() const {
        std::cout << "[" << (isCompleted() ? "X": " ") << "] " << id << ". " << title << "\n";
    }
};