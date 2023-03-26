cmake_minimum_required(VERSION 3.24)
project(untitled C)

set(CMAKE_C_STANDARD 17)

add_executable(untitled main.c)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Rating
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/level_1
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})