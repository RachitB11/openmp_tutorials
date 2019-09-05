cmake_minimum_required(VERSION 2.4)

project(OpenMP_tuts)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(hello_world init/hello_world.cpp)
add_executable(1_integration integration/1_integration.cpp)
add_executable(2_integration_parallelized integration/2_integration_parallelized.cpp)
add_executable(2.1_integration_parallelized_mutex integration/2.1_integration_parallelized_mutex.cpp)
add_executable(3_integration_parallelized_false_sharing integration/3_integration_parallelized_false_sharing.cpp)
add_executable(4_integration_parallelized_critical integration/4_integration_parallelized_critical.cpp)
add_executable(5_integration_reduction integration/5_integration_reduction.cpp)
add_executable(mandel excercises/mandel.cpp)
