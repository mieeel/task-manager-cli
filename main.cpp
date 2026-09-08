#include <iostream>
#include <string>
#include <limits>
#include "TaskManager.hpp"

void menu() {
    std::cout << "\n--- oi miguel ---\n";
    std::cout << "1. adicionar tarefa\n";
    std::cout << "2. listar tarefas\n";
    std::cout << "3. concluir tarefa\n";
    std::cout << "0. sair\n";
    std::cout << "escolha uma opcao: ";
}

int main(){
    TaskManager manager;
    int op = -1;

    while(op != 0){
        menu();
        std::cin >> op;

        if(std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // descarta entrada inválida
            std::cout << "op invalida, digite apenas numeros.\n";
            continue;
        }
    switch(op){
        case 1: {
            std::cout << "titulo: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // limpa buffer

            std::string title;
            std::getline(std::cin, title); // lê toda a linha, incluse espaço

            if(!title.empty()){
                manager.addTask(title);
                std::cout << "👍\n";
            } else{
                std::cout << "👎\n";
            }
            break;
        }
        case 2:
            std::cout << "\n --- tarefas --- \n";
            manager.listTasks();
            break;

        case 3: {
        std::cout << "id: ";
        int id;
        std::cin >> id;

        if(manager.MarkTaskAsCompleted(id)){
            std::cout << "tarefa [" << id << "] " << "concluida.\n";
        } else{
            std::cout << "tarefa [" << id << "] " << "nao encontrada.\n";
        }
        break;
        }
        case 0:
            std::cout << "saindo...\n";
        break;

        default:
            std::cout << "op invalida.\n";
            break;
        }
    }
    return 0;
}