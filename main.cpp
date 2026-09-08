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

// Converte string de prioridade (ex: "alta", "high", "p:1", "p:alta") para Enum Priority
Priority parsePriority(std::string pStr) {
    std::transform(pStr.begin(), pStr.end(), pStr.begin(), ::tolower);
    if (pStr.find("alta") != std::string::npos || pStr.find("high") != std::string::npos || pStr == "3" || pStr == "p:3" || pStr == "p:alta") {
        return Priority::High;
    }
    if (pStr.find("baixa") != std::string::npos || pStr.find("low") != std::string::npos || pStr == "1" || pStr == "p:1" || pStr == "p:baixa") {
        return Priority::Low;
    }
    return Priority::Medium;
}

void printHelp() {
    std::cout << Color::BOLD << "=== GERENCIADOR DE TAREFAS (CLI) ===\n\n" << Color::RESET
              << "Uso:\n"
              << "  task                               Listar apenas tarefas PENDENTES\n"
              << "  tasks                              Listar TODAS as tarefas\n"
              << "  task done                          Listar apenas tarefas CONCLUIDAS\n"
              << "  task add \"Minha tarefa\"           Adicionar tarefa (Prioridade Media padrao)\n"
              << "  task add \"Minha tarefa\" p:alta     Adicionar com prioridade ALTA, MEDIA ou BAIXA\n"
              << "  task x <ID>                        Marcar tarefa como concluida\n"
              << "  task rm <ID>                       Remover permanentemente uma tarefa\n"
              << "  task --help                        Exibir este menu de ajuda\n";
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
    
    if (progName.find("tasks") != std::string::npos && argc < 2) {
        std::cout << Color::BOLD << "--- TODAS AS TAREFAS ---\n" << Color::RESET;
        manager.listAllTasks();
        return 0;
    }

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
        Priority priority = Priority::Medium;
        std::string title = "";

        // Verifica se o último argumento é uma flag de prioridade (ex: p:alta, p:baixa, alta)
        std::string lastArg = argv[argc - 1];
        if (lastArg.rfind("p:", 0) == 0 || lastArg == "alta" || lastArg == "media" || lastArg == "baixa" || lastArg == "high" || lastArg == "low") {
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
        std::cout << Color::GREEN << "✔ Tarefa adicionada com sucesso!\n" << Color::RESET;
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