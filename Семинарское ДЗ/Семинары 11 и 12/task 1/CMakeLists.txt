cmake_minimum_required(VERSION 3.20)
project(task1 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(Task1
    main.cpp
)

target_link_libraries(Task1
    pthread
)
