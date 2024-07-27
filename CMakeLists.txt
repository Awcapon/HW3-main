cmake_minimum_required(VERSION 3.27)
project(Homework_3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Homework_3
        main.cpp
        Person.cpp
        Person.h
        SortedList.h
        Task.cpp
        Task.h
        TaskManager.h)
