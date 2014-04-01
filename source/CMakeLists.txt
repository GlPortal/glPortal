cmake_minimum_required(VERSION 2.8)
set(EXE_NAME portal)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

project(${EXE_NAME})
link_libraries(GLEW GL GLU assimp SDL2 SDL2_mixer BulletDynamics BulletCollision LinearMath)

add_executable(${EXE_NAME} 
  Main.cpp Game.cpp engine/gui/GameScreen.cpp engine/Resources.cpp engine/StringConverter.cpp 
  engine/gui/Character.cpp  engine/gui/Font.cpp engine/tools/Timer.cpp
  engine/object/Model.cpp engine/object/Mesh.cpp
  engine/renderer/GameMapRenderer.cpp 
  map/MapFileParser.cpp
  util/ListFileParser.cpp
  util/ConfigFileParser.cpp
  util/audioplayer/AudioPlayer.cpp
  util/Vector3f.cpp
  engine/Environment.cpp
  Player.cpp GameMap.cpp 
  Shot.cpp Portal.cpp 
  Window.cpp Exception.cpp
  )

add_executable(map
  map/Main.cpp map/MapToWavefront.cpp
  )

add_custom_target(run COMMAND ./${EXE_NAME})
add_custom_target(capture COMMAND avconv -f alsa -f x11grab -r 30 -s 1680x1050 -i :0.0+0,0 -acodec libfaac -vcodec libx264 -pre:0 lossless_ultrafast -threads 0 video.mkv)
add_custom_target(compressVideo COMMAND avconv -i video.mkv -s 1280x800 -c:v libx264 smallVideo.mkv)
include_directories("./lib" "/usr/local/include" "~/system/bin/libs/bullet-2.82-r2704/src")
