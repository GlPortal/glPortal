cmake_minimum_required(VERSION 2.8)
set(EXE_NAME portal)
set(CMAKE_BUILD_TYPE Debug)
project(${EXE_NAME})
link_libraries(GLEW GL GLU glut)

add_executable(${EXE_NAME} 
  Main.cpp Game.cpp Resources.cpp 
  Player.cpp Map.cpp 
  Shot.cpp Portal.cpp 
  Window.cpp
  )

add_executable(map 
  map/Main.cpp map/MapToWavefront.cpp
  )

add_custom_target(run COMMAND ./${EXE_NAME})

include_directories(
  ./lib 
  )
