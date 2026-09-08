#include <iostream>
#include <string>
#include <cstdlib>
#include "TaskManager.hpp"
#include "StorageManager.hpp"

void printHelp() {
    std::cout << "=== GERENCIADOR DE TAREFAS (CLI) ===\n\n"
              << "Uso:\n"
              << "  task                       Listar apenas tarefas PENDENTES\n"
              << "  task -                     Listar apenas tarefas CONCLUIDAS\n"
              << "  task + \"Minha tarefa\"      Adicionar uma nova tarefa\n"
              << "  task x <ID>                Marcar tarefa como concluida pelo ID\n"
              << "  task --help                Exibir este menu de ajuda\n";
}

int main(int argc, char* argv[]) {
    // Definindo o arquivo salvo no diretório do usuário ($HOME ou USERPROFILE)
#ifdef _WIN32
    const char* userHome = std::getenv("USERPROFILE");
    std::string filepath = (userHome ? std::string(userHome) + "\\.tasks.txt" : "tasks.txt");
#else
    const char* userHome = std::getenv("HOME");
    std::string filepath = (userHome ? std::string(userHome) + "/.tasks.txt" : "tasks.txt");
#endif

    TaskManager manager;
    StorageManager storage(filepath);
    storage.load(manager);

    // Sem argumentos: Lista APENAS as PENDENTES
    if (argc < 2) {
        std::cout << "--- TAREFAS PENDENTES ---\n";
        manager.listTasksByStatus(false);
        return 0;
    }

    std::string command = argv[1];

    // Argumento '--help': Exibe ajuda
    if (command == "--help" || command == "-h") {
        printHelp();
    }
    // Argumento '-': Lista APENAS as CONCLUÍDAS
    else if (command == "-") {
        std::cout << "--- TAREFAS CONCLUIDAS ---\n";
        manager.listTasksByStatus(true);
    }
    // Argumento '+': Adiciona nova tarefa
    else if (command == "+" && argc >= 3) {
        std::string title = argv[2];
        for (int i = 3; i < argc; ++i) {
            title += " ";
            title += argv[i];
        }

        manager.addTask(title);
        storage.save(manager);
        std::cout << "✔ Tarefa adicionada: \"" << title << "\"\n";
    }
    // Argumento 'x': Conclui tarefa pelo ID
    else if (command == "x" && argc >= 3) {
        int id = std::stoi(argv[2]);
        if (manager.markTaskCompleted(id)) {
            storage.save(manager);
            std::cout << "✔ Tarefa #" << id << " marcada como concluida!\n";
        } else {
            std::cout << "❌ Tarefa #" << id << " nao encontrada.\n";
        }
    } 
    else {
        std::cout << "Comando nao reconhecido. Use 'task --help' para ver as opcoes.\n";
    }

    return 0;
}