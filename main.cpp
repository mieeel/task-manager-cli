#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <vector>
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
              << "  task events                List ONLY scheduled events and deadlines\n"
              << "  task search <keyword>      Search tasks by title keyword\n"
              << "  task stats                 View task completion statistics\n"
              << "  task add \"Meeting\" due:2026-09-15T14:00 Add a scheduled task/event\n"
              << "  task add \"Title\" @tag p:high Add a task with tags and priority\n"
              << "  task edit <ID> due:2026-09-20 Update due date/time for a task\n"
              << "  task done <ID1> <ID2> ...  Mark task(s) as completed\n"
              << "  task rm <ID1> <ID2> ...    Remove task(s) permanently\n"
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

    if (argc < 2) {
        std::cout << Color::BOLD << "--- PENDING TASKS ---\n" << Color::RESET;
        manager.listTasksByStatus(false);
        return 0;
    }

    std::string command = argv[1];

    if (command == "--help" || command == "-h" || command == "help") {
        printHelp();
    }
    else if (command == "events" || command == "schedule") {
        manager.listEventsOnly();
    }
    else if (command == "stats") {
        manager.printStats();
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
            } else if ((subflag == "--prio" || subflag == "-p") && argc >= 4) {
                Priority priority = parsePriority(argv[3]);
                std::cout << Color::BOLD << "--- TASKS BY PRIORITY ---\n" << Color::RESET;
                manager.listTasksByPriority(priority);
            } else if (subflag.rfind("@", 0) == 0) {
                std::string tag = subflag.substr(1);
                std::cout << Color::BOLD << "--- TASKS WITH TAG #" << tag << " ---\n" << Color::RESET;
                manager.listTasksByTag(tag);
            } else {
                std::cout << Color::YELLOW << "Unknown flag. Use 'task --help' for details.\n" << Color::RESET;
            }
        } else {
            std::cout << Color::BOLD << "--- PENDING TASKS ---\n" << Color::RESET;
            manager.listTasksByStatus(false);
        }
    }
    else if (command == "search" && argc >= 3) {
        std::string query = argv[2];
        for (int i = 3; i < argc; ++i) {
            query += " ";
            query += argv[i];
        }
        std::cout << Color::BOLD << "--- SEARCH RESULTS FOR \"" << query << "\" ---\n" << Color::RESET;
        manager.searchTasks(query);
    }
    else if (command == "add" && argc >= 3) {
        Priority priority = Priority::Medium;
        int parentId = 0;
        std::vector<std::string> tags;
        std::string dueDate = "";
        std::string title = "";

        for (int i = 2; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg.rfind("p:", 0) == 0 || arg == "high" || arg == "med" || arg == "low") {
                priority = parsePriority(arg);
            } else if (arg.rfind("sub:", 0) == 0) {
                try { parentId = std::stoi(arg.substr(4)); } catch(...) {}
            } else if (arg.rfind("due:", 0) == 0) {
                dueDate = arg.substr(4);
            } else if (arg.rfind("@", 0) == 0) {
                tags.push_back(arg.substr(1));
            } else {
                if (!title.empty()) title += " ";
                title += arg;
            }
        }

        manager.addTask(title, priority, parentId, tags, dueDate);
        storage.save(manager);
        std::cout << Color::GREEN << "✔ Task added successfully!\n" << Color::RESET;
    }
    else if (command == "edit" && argc >= 4) {
        try {
            int id = std::stoi(argv[2]);
            Priority priority = Priority::Medium;
            std::vector<std::string> tags;
            std::string dueDate = "";
            std::string title = "";
            bool updatePriority = false;
            bool updateTitle = false;
            bool updateTags = false;
            bool updateDueDate = false;

            for (int i = 3; i < argc; ++i) {
                std::string arg = argv[i];
                if (arg.rfind("p:", 0) == 0) {
                    priority = parsePriority(arg);
                    updatePriority = true;
                } else if (arg.rfind("due:", 0) == 0) {
                    dueDate = arg.substr(4);
                    updateDueDate = true;
                } else if (arg.rfind("@", 0) == 0) {
                    tags.push_back(arg.substr(1));
                    updateTags = true;
                } else {
                    if (!title.empty()) title += " ";
                    title += arg;
                }
            }

            if (!title.empty()) updateTitle = true;

            if (manager.editTask(id, title, priority, tags, dueDate, updateTitle, updatePriority, updateTags, updateDueDate)) {
                storage.save(manager);
                std::cout << Color::GREEN << "✔ Task #" << std::setw(3) << std::setfill('0') << id << " updated successfully!\n" << Color::RESET;
            } else {
                std::cout << Color::RED << "❌ Task #" << std::setw(3) << std::setfill('0') << id << " not found.\n" << Color::RESET;
            }
        } catch (...) {
            std::cout << Color::RED << "❌ Invalid ID provided.\n" << Color::RESET;
        }
    }
    else if (command == "done" || command == "x") {
        if (argc < 3) {
            std::cout << Color::RED << "❌ Please provide at least one task ID.\n" << Color::RESET;
            return 1;
        }
        for (int i = 2; i < argc; ++i) {
            try {
                int id = std::stoi(argv[i]);
                if (manager.markTaskCompleted(id)) {
                    std::cout << Color::GREEN << "✔ Task #" << std::setw(3) << std::setfill('0') << id << " completed!\n" << Color::RESET;
                } else {
                    std::cout << Color::RED << "❌ Task #" << std::setw(3) << std::setfill('0') << id << " not found.\n" << Color::RESET;
                }
            } catch (...) {
                std::cout << Color::RED << "❌ Invalid ID: " << argv[i] << "\n" << Color::RESET;
            }
        }
        storage.save(manager);
    } 
    else if (command == "rm" || command == "del") {
        if (argc < 3) {
            std::cout << Color::RED << "❌ Please provide at least one task ID.\n" << Color::RESET;
            return 1;
        }
        for (int i = 2; i < argc; ++i) {
            try {
                int id = std::stoi(argv[i]);
                if (manager.deleteTask(id)) {
                    std::cout << Color::RED << "🗑️ Task #" << std::setw(3) << std::setfill('0') << id << " removed!\n" << Color::RESET;
                } else {
                    std::cout << Color::RED << "❌ Task #" << std::setw(3) << std::setfill('0') << id << " not found.\n" << Color::RESET;
                }
            } catch (...) {
                std::cout << Color::RED << "❌ Invalid ID: " << argv[i] << "\n" << Color::RESET;
            }
        }
        storage.save(manager);
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