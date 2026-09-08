#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

#include <vector>
#include <algorithm>
#include <iostream>
#include "Task.hpp"

class TaskManager {
private:
    std::vector<Task> tasks;
    int nextId = 1;

public:
    // Retorna a referência constante do vetor para leitura/salvamento no arquivo
    const std::vector<Task>& getTasks() const {
        return tasks;
    }

    // Adiciona uma nova tarefa via comando do usuário
    void addTask(const std::string& title) {
        tasks.emplace_back(nextId++, title);
    }

    // Recria uma tarefa existente vinda do arquivo mantendo seu ID e status
    void loadTask(int id, const std::string& title, bool completed) {
        tasks.emplace_back(id, title);
        if (completed) {
            tasks.back().markAsCompleted();
        }
        // Garante que o próximo ID gerado seja maior que o maior ID já carregado
        if (id >= nextId) {
            nextId = id + 1;
        }
    }

    // Lista todas as tarefas cadastradas no terminal
    void listTasks() const {
        if (tasks.empty()) {
            std::cout << "Nenhuma tarefa cadastrada.\n";
            return;
        }
        for (const auto& task : tasks) {
            task.print();
        }
    }

    // Marca uma tarefa como concluída pelo seu ID
    bool markTaskCompleted(int id) {
        auto it = std::find_if(tasks.begin(), tasks.end(), [id](const Task& t) {
            return t.getId() == id;
        });

        if (it != tasks.end()) {
            it->markAsCompleted();
            return true;
        }
        return false;
    }
};

#endif