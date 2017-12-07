cmake_minimum_required(VERSION 3.1)
project(yandex-test-task-cpp)
set (CMAKE_CXX_STANDARD 11)

#set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../build)

add_executable(check_mem_usage check_mem_usage.cpp)

add_executable(rand_num_base_generator rand_num_base_generator.cpp)

add_executable(goforbroke1006_sort goforbroke1006_sort.cpp)

add_executable(stepic_derivative stepic-derivative.cpp)