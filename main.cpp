#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
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

void printHelp() {
    std::cout << Color::BOLD << "=== GERENCIADOR DE TAREFAS (CLI) ===\n\n" << Color::RESET
              << "Uso:\n"
              << "  task                       Listar apenas tarefas PENDENTES\n"
              << "  tasks                      Listar TODAS as tarefas\n"
              << "  task done                  Listar apenas tarefas CONCLUIDAS\n"
              << "  task add \"Minha tarefa\"   Adicionar uma nova tarefa\n"
              << "  task x <ID>                Marcar tarefa como concluida\n"
              << "  task rm <ID>               Remover permanentemente uma tarefa\n"
              << "  task --help                Exibir este menu de ajuda\n";
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

    std::string progName = argv[0];
    
    // Suporte ao comando 'tasks' para listar tudo diretamente
    if (progName.find("tasks") != std::string::npos && argc < 2) {
        std::cout << Color::BOLD << "--- TODAS AS TAREFAS ---\n" << Color::RESET;
        manager.listAllTasks();
        return 0;
    }

    // Executável chamado como 'task' sem argumentos: Pendentes
    if (argc < 2) {
        std::cout << Color::BOLD << "--- TAREFAS PENDENTES ---\n" << Color::RESET;
        manager.listTasksByStatus(false);
        return 0;
    }

    std::string command = argv[1];

    if (command == "--help" || command == "-h") {
        printHelp();
    }
    else if (command == "done") {
        std::cout << Color::BOLD << "--- TAREFAS CONCLUIDAS ---\n" << Color::RESET;
        manager.listTasksByStatus(true);
    }
    else if (command == "add" && argc >= 3) {
        std::string title = argv[2];
        for (int i = 3; i < argc; ++i) {
            title += " ";
            title += argv[i];
        }

        manager.addTask(title);
        storage.save(manager);
        std::cout << Color::GREEN << "✔ Tarefa adicionada: \"" << title << "\"\n" << Color::RESET;
    }
    else if (command == "x" && argc >= 3) {
        try {
            int id = std::stoi(argv[2]);
            if (manager.markTaskCompleted(id)) {
                storage.save(manager);
                std::cout << Color::GREEN << "✔ Tarefa #" << std::setw(3) << std::setfill('0') << id << " marcada como concluida!\n" << Color::RESET;
            } else {
                std::cout << Color::RED << "❌ Tarefa #" << std::setw(3) << std::setfill('0') << id << " nao encontrada.\n" << Color::RESET;
            }
        } catch (...) {
            std::cout << Color::RED << "❌ ID invalido fornecido.\n" << Color::RESET;
        }
    } 
    else if ((command == "rm" || command == "del") && argc >= 3) {
        try {
            int id = std::stoi(argv[2]);
            if (manager.deleteTask(id)) {
                storage.save(manager);
                std::cout << Color::RED << "🗑️ Tarefa #" << std::setw(3) << std::setfill('0') << id << " removida com sucesso!\n" << Color::RESET;
            } else {
                std::cout << Color::RED << "❌ Tarefa #" << std::setw(3) << std::setfill('0') << id << " nao encontrada.\n" << Color::RESET;
            }
        } catch (...) {
            std::cout << Color::RED << "❌ ID invalido fornecido.\n" << Color::RESET;
        }
    }
    else {
        std::cout << Color::YELLOW << "Comando nao reconhecido. Use 'task --help' para ver as opcoes.\n" << Color::RESET;
    }

    return 0;
}