cmake_minimum_required(VERSION 3.3)
project(project2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

set(SOURCE_FILES main.cpp Node.h evaluator.h)
add_executable(project2 ${SOURCE_FILES})