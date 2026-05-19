cmake_minimum_required(VERSION 3.10.0)
project(task1 VERSION 0.1.0 LANGUAGES C CXX)
cmake_minimum_required(VERSION 3.20)

project(task1)

set(CMAKE_CXX_STANDARD 20)

find_package(TBB REQUIRED)

add_executable(task1 main.cpp Dispatcher.cpp ProcessingPool.cpp ResultAggregator.cpp SensorSimulator.cpp Storage.cpp)

target_link_libraries(task1 PRIVATE TBB::tbb)
