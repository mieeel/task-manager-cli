#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include "TaskManager.hpp"
#include "StorageManager.hpp"
#include "Colors.hpp"

#ifdef _WIN32
#include <windows.h>

void enableWindowsANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#endif

Priority parsePriority(std::string pStr) {
    std::transform(pStr.begin(), pStr.end(), pStr.begin(), ::tolower);
    if (pStr.find("high") != std::string::npos || pStr == "3" || pStr == "p:3" || pStr == "p:high") {
        return Priority::High;
    }
    if (pStr.find("low") != std::string::npos || pStr == "1" || pStr == "p:1" || pStr == "p:low") {
        return Priority::Low;
    }
    return Priority::Medium;
}

void printHelp() {
    std::cout << Color::BOLD << "=== TASK MANAGER CLI ===\n\n" << Color::RESET
              << "Usage:\n"
              << "  task [list|ls]             List pending tasks (Default)\n"
              << "  task list --all (-a)       List ALL tasks\n"
              << "  task list --done           List COMPLETED tasks\n"
              << "  task add \"Task title\"     Add a new task (Medium priority by default)\n"
              << "  task add \"Task title\" p:high Add a task with HIGH, MED, or LOW priority\n"
              << "  task done <ID>             Mark task as completed (alias: task x <ID>)\n"
              << "  task rm <ID>               Remove a task permanently (alias: task del <ID>)\n"
              << "  task clear                 Remove all completed tasks\n"
              << "  task --help (-h)           Show this help menu\n";
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    enableWindowsANSI();
    const char* userHome = std::getenv("USERPROFILE");
    std::string filepath = (userHome ? std::string(userHome) + "\\.tasks.txt" : "tasks.txt");
#else
    const char* userHome = std::getenv("HOME");
    std::string filepath = (userHome ? std::string(userHome) + "/.tasks.txt" : "tasks.txt");
#endif

    TaskManager manager;
    StorageManager storage(filepath);
    storage.load(manager);

    // Default command (no arguments provided)
    if (argc < 2) {
        std::cout << Color::BOLD << "--- PENDING TASKS ---\n" << Color::RESET;
        manager.listTasksByStatus(false);
        return 0;
    }

    std::string command = argv[1];

    if (command == "--help" || command == "-h" || command == "help") {
        printHelp();
    }
    else if (command == "list" || command == "ls") {
        if (argc >= 3) {
            std::string subflag = argv[2];
            if (subflag == "--all" || subflag == "-a") {
                std::cout << Color::BOLD << "--- ALL TASKS ---\n" << Color::RESET;
                manager.listAllTasks();
            } else if (subflag == "--done") {
                std::cout << Color::BOLD << "--- COMPLETED TASKS ---\n" << Color::RESET;
                manager.listTasksByStatus(true);
            } else {
                std::cout << Color::YELLOW << "Unknown flag. Use 'task --help' for details.\n" << Color::RESET;
            }
        } else {
            std::cout << Color::BOLD << "--- PENDING TASKS ---\n" << Color::RESET;
            manager.listTasksByStatus(false);
        }
    }
    else if (command == "add" && argc >= 3) {
        Priority priority = Priority::Medium;
        std::string title = "";

        std::string lastArg = argv[argc - 1];
        if (lastArg.rfind("p:", 0) == 0 || lastArg == "high" || lastArg == "med" || lastArg == "low") {
            priority = parsePriority(lastArg);
            for (int i = 2; i < argc - 1; ++i) {
                if (i > 2) title += " ";
                title += argv[i];
            }
        } else {
            for (int i = 2; i < argc; ++i) {
                if (i > 2) title += " ";
                title += argv[i];
            }
        }

        manager.addTask(title, priority);
        storage.save(manager);
        std::cout << Color::GREEN << "✔ Task added successfully!\n" << Color::RESET;
    }
    else if ((command == "done" || command == "x") && argc >= 3) {
        try {
            int id = std::stoi(argv[2]);
            if (manager.markTaskCompleted(id)) {
                storage.save(manager);
                std::cout << Color::GREEN << "✔ Task #" << std::setw(3) << std::setfill('0') << id << " marked as completed!\n" << Color::RESET;
            } else {
                std::cout << Color::RED << "❌ Task #" << std::setw(3) << std::setfill('0') << id << " not found.\n" << Color::RESET;
            }
        } catch (...) {
            std::cout << Color::RED << "❌ Invalid ID provided.\n" << Color::RESET;
        }
    } 
    else if ((command == "rm" || command == "del") && argc >= 3) {
        try {
            int id = std::stoi(argv[2]);
            if (manager.deleteTask(id)) {
                storage.save(manager);
                std::cout << Color::RED << "🗑️ Task #" << std::setw(3) << std::setfill('0') << id << " removed successfully!\n" << Color::RESET;
            } else {
                std::cout << Color::RED << "❌ Task #" << std::setw(3) << std::setfill('0') << id << " not found.\n" << Color::RESET;
            }
        } catch (...) {
            std::cout << Color::RED << "❌ Invalid ID provided.\n" << Color::RESET;
        }
    }
    else if (command == "clear") {
        int removedCount = manager.clearCompletedTasks();
        if (removedCount > 0) {
            storage.save(manager);
            std::cout << Color::GREEN << "🧹 Removed " << removedCount << " completed task(s).\n" << Color::RESET;
        } else {
            std::cout << Color::GRAY << "No completed tasks to clear.\n" << Color::RESET;
        }
    }
    else {
        std::cout << Color::YELLOW << "Unknown command. Use 'task --help' to view available options.\n" << Color::RESET;
    }

    return 0;
}