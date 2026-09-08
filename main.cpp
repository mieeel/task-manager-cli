#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include "TaskManager.hpp"
#include "StorageManager.hpp"

const char* homeDir = std::getenv("HOME");



int main(int argc, char* argv[]){
    std::string filepath = (homeDir ? std::string(homeDir) + "/.tasks.txt" : "tasks.txt");

    TaskManager manager;
    StorageManager storage(filepath);

    storage.load(manager);

    if (argc < 2){
        std::cout << "\n--- suas tarefas ---\n";
        manager.listTasks();
        std::cout << "\ncomo usar:\n";
        std::cout << "  task                   -> listar tarefas\n";
        std::cout << "  task + \"nome tarefa\" -> adicionar tarefa\n";
        std::cout << "  task x 2               -> concluir tarefa pelo ID\n";
        return 0;
    }

    std::string command = argv[1];

    // adicionar
    if(command == "+" && argc >= 3){
        std::string title = argv[2];
        // Se o usuário digitou palavras separadas sem aspas (ex: task + fazer lição)
        for (int i = 3; i < argc; ++i) {
            title += " ";
            title += argv[i];
        }
        manager.addTask(title);
        storage.save(manager); // Salva no arquivo
        std::cout << "✔ tarefa adicionada: \"" << title << "\"\n";
    }
    // concluir (x)
    else if (command == "x" && argc >= 3) {
        int id = std::stoi(argv[2]);
        if (manager.markTaskCompleted(id)) {
            storage.save(manager); // salva a alteração
            std::cout << "✔ tarefa #" << id << " concluida.\n";
        } else {
            std::cout << "❌ tarefa #" << id << " nao encontrada.\n";
        }
    } 
    else {
        std::cout << "comando invalido.\n";
    }

    return 0;
}