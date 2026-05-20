cmake_minimum_required(VERSION 3.20)

project(task1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_COMPILER
    /opt/intel/oneapi/compiler/latest/bin/dpcpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_executable(task1 main.cpp)

target_compile_options(task1 PRIVATE -fsycl)
target_link_options(task1 PRIVATE -fsycl)
