#include "TaskManager.h"
#include "Person.h"
#include "Task.h"
#include <iostream>
#include <string>

TaskManager::TaskManager() : numPersons(0) {

    personsArray = new Person*[MAX_PERSONS];

    for (int i = 0; i < MAX_PERSONS; ++i) {

        personsArray[i] = nullptr;

    }

}

TaskManager::~TaskManager() {

    for (int i = 0; i < MAX_PERSONS; ++i) {

        delete personsArray[i];

    }

    delete[] personsArray;

}


void TaskManager::assignTask(const std::string& personName, const Task& task) {

    static int taskId = 0;

    if (personName.empty()) {

        std::cout << "Error: Person name cannot be empty." << std::endl;

        return;

    }

    for (int i = 0; i < numPersons; ++i) {

        if (personsArray[i] && personsArray[i]->getName() == personName) {

            Task newTask = task;

            newTask.setId(taskId++);

            personsArray[i]->assignTask(newTask);

            return;
        }
    }

    if (numPersons < MAX_PERSONS) {

        personsArray[numPersons] = new Person(personName);

        Task newTask = task;

        newTask.setId(taskId++);

        personsArray[numPersons]->assignTask(newTask);

        ++numPersons;

    } else {

        std::cout << "Error: Maximum number of persons reached." << std::endl;

    }
}


void TaskManager::completeTask(const std::string& personName) {

    for (int i = 0; i < numPersons; ++i) {

        if (personsArray[i] && personsArray[i]->getName() == personName) {

            if (personsArray[i]->getTasks().length() > 0) {

                personsArray[i]->completeTask();

            } else {

                std::cout << "Error: No tasks to complete for " << personName <<
                "." << std::endl;
            }
            return;
        }
    }

    std::cout << "Error: Person not found." << std::endl;
}

void TaskManager::bumpPriorityByType(TaskType type, int priority) {

    for (int i = 0; i < numPersons; ++i) {

        if (personsArray[i]) {

            personsArray[i]->bumpPriorityByType(type, priority);

        }
    }
}

void TaskManager::printAllEmployees() const {

    if (numPersons == 0) {

        std::cout << "No employees." << std::endl;

        return;
    }

    for (int i = 0; i < numPersons; ++i) {

        if (personsArray[i]) {

            std::cout << *personsArray[i] << std::endl;

        }
    }
}

void TaskManager::printTasksByType(TaskType type) const {

    bool tasksFound = false;

    for (int i = 0; i < numPersons; ++i) {

        if (personsArray[i]) {

            if (personsArray[i]->printTasksByType(type)) {

                tasksFound = true;

            }
        }
    }

    if (!tasksFound) {

        std::cout << "No tasks of type " << type << " found." << std::endl;

    }
}


void TaskManager::printAllTasks() const {

    bool tasksFound = false;

    for (int i = 0; i < numPersons; ++i) {

        if (personsArray[i]) {

            if (personsArray[i]->printTasks()) {

                tasksFound = true;
            }
        }
    }

    if (!tasksFound) {

        std::cout << "No tasks found." << std::endl;
    }
}
