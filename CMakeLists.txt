cmake_minimum_required(VERSION 2.8)
set(EXE_NAME portal)
set(CMAKE_BUILD_TYPE Debug)

project(${EXE_NAME})

add_subdirectory(data)
add_subdirectory(source)
