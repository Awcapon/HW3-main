#include "TaskManager.h"

TaskManager::TaskManager() : numPersons(0), taskId(0) {

    for (int i = 0; i < MAX_PERSONS; ++i) {

        personsArray[i] = nullptr;

    }
}

void TaskManager::assignTask(const std::string& personName, const Task& task) {

    for (int i = 0; i < numPersons; ++i) {

        if (personsArray[i] != nullptr && personsArray[i]->getName() == personName) {

            Task newTask(task.getPriority(), task.getType(), task.getDescription());

            newTask.setId(taskId++);

            personsArray[i]->assignTask(newTask);

            return;

        }
    }

    if (numPersons >= MAX_PERSONS) {

        throw std::runtime_error("Error: Maximum number of persons reached.");

    }

    for (int i = 0; i < numPersons; ++i) {

        if (personsArray[i] != nullptr && personsArray[i]->getName() == personName) {

            delete personsArray[i];

            personsArray[i] = nullptr;

            break;
        }
    }

    personsArray[numPersons] = new Person(personName);

    Task newTask(task.getPriority(), task.getType(), task.getDescription());

    newTask.setId(taskId++);

    personsArray[numPersons]->assignTask(newTask);

    ++numPersons;

}




void TaskManager::completeTask(const std::string& personName) {

    for (int i = 0; i < numPersons; ++i) {

        if (personsArray[i] != nullptr && personsArray[i]->getName() == personName) {

            personsArray[i]->completeTask();

            return;

        }
    }

}

void TaskManager::bumpPriorityByType(TaskType type, int priorityBump) {

    if (priorityBump < 0) {

        return;

    }

    for (int i = 0; i < numPersons; i++) {

        if (personsArray[i]!= nullptr) {

            SortedList<Task> tasks = personsArray[i]->getTasks();

            SortedList<Task> newTasks;

            for (const Task& task : tasks) {

                if (task.getType() == type) {

                    int newPriority = task.getPriority() + priorityBump;

                    if (newPriority > 100) {

                        newPriority = 100;

                    }
                    Task updatedTask(newPriority, task.getType(), task.getDescription());

                    updatedTask.setId(task.getId());


                    newTasks.insert(updatedTask);

                } else {

                    newTasks.insert(task);

                }
            }

            personsArray[i]->setTasks(newTasks);
        }
    }
}

void TaskManager::printAllEmployees() const {

    for (int i = 0; i < numPersons; ++i) {

        if (personsArray[i]!= nullptr) {

            std::cout << *(personsArray[i]) << std::endl;

        }
    }
}

void TaskManager::printAllTasks() const {

    SortedList<Task> allTasks;

    for (int i = 0; i < numPersons; ++i) {

        if (personsArray[i]!= nullptr) {

            const SortedList<Task>& tasks = personsArray[i]->getTasks();


            for (const Task& task : tasks) {

                allTasks.insert(task);
            }
        }
    }

    for (const Task& task : allTasks) {

        std::cout << task << std::endl;

    }

}

void TaskManager::printTasksByType(TaskType type) const {

    SortedList<Task> tasksByType;

    for (int i = 0; i < numPersons; ++i) {

        if (personsArray[i]!= nullptr) {

            const SortedList<Task>& tasks = personsArray[i]->getTasks();

            for (const Task& task : tasks) {

                if (task.getType() == type) {

                    tasksByType.insert(task);

                }

            }
        }
    }

    for (const Task& task : tasksByType) {

        std::cout << task << std::endl;

    }
}