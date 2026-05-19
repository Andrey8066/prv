cmake_minimum_required(VERSION 3.20)
project(task2 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(Task2
    main.cpp
    BankAccount.cpp
)

target_link_libraries(Task2
    pthread
)
