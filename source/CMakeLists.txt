cmake_minimum_required(VERSION 2.8)
set(EXE_NAME portal)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")

project(${EXE_NAME})
link_libraries(GLEW GL GLU assimp SDL2)

add_executable(${EXE_NAME} 
  Main.cpp Game.cpp engine/gui/GameScreen.cpp engine/Resources.cpp engine/StringConverter.cpp 
  engine/gui/Character.cpp  engine/gui/Font.cpp engine/tools/Timer.cpp
  engine/object/Model.cpp engine/object/Mesh.cpp
  map/MapFileParser.cpp
  Player.cpp GameMap.cpp 
  Shot.cpp Portal.cpp 
  Window.cpp Exception.cpp
  )

add_executable(map
  map/Main.cpp map/MapToWavefront.cpp
  )

add_custom_target(run COMMAND ./${EXE_NAME})

include_directories(./lib)
