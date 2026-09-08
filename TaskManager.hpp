#include <vector>
#include <algorithm>
#include "Task.hpp"

class TaskManager {
    private:
    std::vector<Task> tasks;
    int nextID = 1;

    public:
    void addTask(const std::string& title) {
        tasks.emplace_back(nextID++, title); // usar emplace_back ao invés de push_back faz criar o objeto direto no vetor
    }

    void listTasks() const {
        if(tasks.empty()) {
            std::cout << "nenhuma tarefa cadastrada.\n";
            return;
        }
        for(const auto& task : tasks){
            task.print();
        }
    }

    bool MarkTaskAsCompleted(int id) {
        auto it = std::find_if(tasks.begin(), tasks.end(), [id](const Task& t) {
            return t.getID() == id;
        });

        if(it != tasks.end()) {
            it->markAsCompleted();
            return true;
        }
        return false;
    }
};