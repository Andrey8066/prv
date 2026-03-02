cmake_minimum_required(VERSION 3.10)


project(BankProject LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(task2
    task2.cpp
    BankAtomic.cpp
    BankMutex.cpp
    Bank.cpp
)
find_package(Threads REQUIRED)
target_link_libraries(task2 PRIVATE Threads::Threads)
