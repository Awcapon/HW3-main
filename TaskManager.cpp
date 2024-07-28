#include "TaskManager.h"


TaskManager::TaskManager() : numPersons(0), taskId(0) {

    personsArray = new Person*[MAX_PERSONS]();

}

TaskManager::~TaskManager() {

    for (int i = 0; i < numPersons; i++) {

        delete personsArray[i];

    }

    delete[] personsArray;
}

void TaskManager::assignTask(const std::string& personName, const Task& task) {

    for (int i = 0; i < numPersons; i++) {

        if (personsArray[i] != nullptr && personsArray[i]->getName() ==
        personName) {
            Task newTask(taskId++, task.getType(),
                         task.getDescription());

            personsArray[i]->assignTask(newTask);

            return;
        }
    }

    if (numPersons >= MAX_PERSONS) {

        throw std::runtime_error("Error: Maximum number of persons reached.");

    }

    Person* newPerson = new Person(personName);

    Task newTask(taskId++, task.getType(), task.getDescription());

    newPerson->assignTask(newTask);

    personsArray[numPersons++] = newPerson;

}

void TaskManager::completeTask(const std::string& personName) {

    for (int i = 0; i < numPersons; i++) {

        if (personsArray[i] != nullptr && personsArray[i]->getName() == personName) {

            personsArray[i]->completeTask();

            return;
        }
    }
    std::cout << "Error: No person with name " << personName << " found." << std::endl;
}

void TaskManager::bumpPriorityByType(TaskType type, int priorityBump) {

    for (int i = 0; i < numPersons; i++) {

        if (personsArray[i] != nullptr) {

            SortedList<Task> tasks = personsArray[i]->getTasks();

            for (auto it = tasks.begin(); it != tasks.end(); ++it) {

                Task task = *it;

                if (task.getType() == type) {

                    int newPriority = task.getPriority() + priorityBump;

                    tasks.remove(it);

                    Task newTask(newPriority, task.getType(), task.getDescription());

                    tasks.insert(newTask);
                }
            }
            personsArray[i]->setTasks(tasks);
        }
    }
}




void TaskManager::printAllEmployees() const {

    for (int i = 0; i < numPersons; i++) {

        if (personsArray[i] != nullptr) {

            std::cout << *personsArray[i] << std::endl;

        }
    }
}

void TaskManager::printAllTasks() const {

    for (int i = 0; i < numPersons; i++) {

        if (personsArray[i] != nullptr) {

            const SortedList<Task>& tasks = personsArray[i]->getTasks();

            for (const Task& task : tasks) {

                std::cout << task << std::endl;
            }
        }
    }
}

void TaskManager::printTasksByType(TaskType type) const {

    for (int i = 0; i < numPersons; i++) {

        if (personsArray[i] != nullptr) {

            const SortedList<Task>& tasks = personsArray[i]->getTasks();

            for (const Task& task : tasks) {

                if (task.getType() == type) {

                    std::cout << task << std::endl;

                }
            }
        }
    }

}
