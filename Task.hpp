#ifndef TASK_HPP
#define TASK_HPP

#include <string>
#include <vector>
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
    int parentId;
    std::vector<std::string> tags;
    std::string dueDate; // Formato: YYYY-MM-DD ou YYYY-MM-DD HH:MM (vazio se nao for evento/prazo)

public:
    Task(int id, const std::string& title, Priority priority = Priority::Medium, int parentId = 0, const std::vector<std::string>& tags = {}, const std::string& dueDate = "") 
        : id(id), title(title), status(TaskStatus::Pending), priority(priority), parentId(parentId), tags(tags), dueDate(dueDate) {}

    int getId() const { return id; }
    std::string getTitle() const { return title; }
    bool isCompleted() const { return status == TaskStatus::Completed; }
    Priority getPriority() const { return priority; }
    int getParentId() const { return parentId; }
    const std::vector<std::string>& getTags() const { return tags; }
    std::string getDueDate() const { return dueDate; }
    bool hasDueDate() const { return !dueDate.empty(); }

    void setTitle(const std::string& newTitle) { title = newTitle; }
    void setPriority(Priority newPriority) { priority = newPriority; }
    void setTags(const std::vector<std::string>& newTags) { tags = newTags; }
    void setDueDate(const std::string& newDueDate) { dueDate = newDueDate; }

    void markAsCompleted() { 
        status = TaskStatus::Completed; 
    }

    bool hasTag(const std::string& tag) const {
        for (const auto& t : tags) {
            if (t == tag) return true;
        }
        return false;
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

    void print(bool isSubtask = false) const {
        std::string indent = isSubtask ? "    └── " : "";
        
        std::string tagsStr = "";
        for (const auto& tag : tags) {
            tagsStr += Color::MAGENTA + " #" + tag + Color::RESET;
        }

        std::string dateStr = "";
        if (hasDueDate()) {
            dateStr = Color::BLUE + " 📅 " + dueDate + Color::RESET;
        }

        if (isCompleted()) {
            std::cout << indent << Color::GREEN << "[X] " << Color::RESET
                      << Color::GRAY << std::setw(3) << std::setfill('0') << id << ". "
                      << "[DONE] " << title << tagsStr << dateStr << Color::RESET << "\n";
        } else {
            std::cout << indent << Color::YELLOW << "[ ] " << Color::RESET
                      << Color::BOLD << std::setw(3) << std::setfill('0') << id << ". " << Color::RESET
                      << getColoredPriority() << " "
                      << Color::CYAN << title << Color::RESET
                      << tagsStr << dateStr << "\n";
        }
    }
};

#endif