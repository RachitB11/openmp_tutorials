cmake_minimum_required(VERSION 2.4)

project(OpenMP_tuts)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(hello_world hello_world.cpp)
add_executable(1_integration integration_false_sharing/1_integration.cpp)
add_executable(2_integration_parallelized integration_false_sharing/2_integration_parallelized.cpp)
add_executable(2.1_integration_parallelized_mutex integration_false_sharing/2.1_integration_parallelized_mutex.cpp)
add_executable(3_integration_parallelized_false_sharing integration_false_sharing/3_integration_parallelized_false_sharing.cpp)
