cmake_minimum_required(VERSION 3.1)
project(yandex-test-task-cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11")

add_executable(goforbroke1006_sort gfb/main.cpp gfb/gfb_file.h gfb/gfb_ram_control.h gfb/gfb_base_file_mng.h gfb/gfb_measure.h)