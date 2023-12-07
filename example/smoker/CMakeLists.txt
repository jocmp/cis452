cmake_minimum_required(VERSION 3.5)
project(smoker)

find_package(Curses REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    gv_semaphore.cpp
    gv_semaphore.h
    smoker.cpp)

add_executable(smoker ${SOURCE_FILES})
target_link_libraries(smoker ${CURSES_LIBRARIES})
