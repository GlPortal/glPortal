cmake_minimum_required(VERSION 2.8)
set (EXE_NAME portal)
set (CMAKE_BUILD_TYPE Debug)
project(${EXE_NAME})
link_libraries(GL GLU glut)
add_executable(${EXE_NAME} Main.cpp Player.cpp Map.cpp Shot.cpp Portal.cpp)

add_custom_target(run COMMAND ./${EXE_NAME})
