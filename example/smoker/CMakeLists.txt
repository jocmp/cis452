cmake_minimum_required(VERSION 3.3)
project(smoker)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    gv_semaphore.cpp
    gv_semaphore.h
    smoker.cpp)

add_executable(smoker ${SOURCE_FILES})