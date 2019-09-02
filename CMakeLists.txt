cmake_minimum_required(VERSION 2.4)

project(OpenMP_tuts)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(hello_world hello_world.cpp)
add_executable(integration integration.cpp)
add_executable(integration_parallelized integration_parallelized.cpp)
add_executable(integration_parallelized_mutex integration_parallelized_mutex.cpp)
