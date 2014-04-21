cmake_minimum_required(VERSION 2.8)
set(EXE_NAME portal)
set(CMAKE_BUILD_TYPE Debug)



project(${EXE_NAME})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(CXX11 REQUIRED)
set(CMAKE_CXX_FLAGS "${CXX11_FLAGS} ${CMAKE_CXX_FLAGS}")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})
set( PORTAL_LIBRARIES ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} )

find_package(Assimp REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIRS})
list( APPEND PORTAL_LIBRARIES ${ASSIMP_LIBRARIES} )

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIRS})
list( APPEND PORTAL_LIBRARIES ${SDL2_LIBRARY} ${SDL2_MIXER_LIBRARIES} )

add_executable(${EXE_NAME} 
  Main.cpp Game.cpp engine/gui/GameScreen.cpp engine/Resources.cpp engine/StringConverter.cpp 
  engine/gui/Character.cpp  engine/gui/Font.cpp engine/tools/Timer.cpp
  engine/object/Model.cpp engine/object/Mesh.cpp
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
target_link_libraries(${EXE_NAME} ${PORTAL_LIBRARIES})
  
add_executable(map
  map/Main.cpp map/MapToWavefront.cpp
  )
target_link_libraries(map ${PORTAL_LIBRARIES})


add_custom_target(run COMMAND ${EXE_NAME})
add_custom_target(capture COMMAND avconv -f alsa -f x11grab -r 30 -s 1680x1050 -i :0.0+0,0 -acodec libfaac -vcodec libx264 -pre:0 lossless_ultrafast -threads 0 video.mkv)
add_custom_target(compressVideo COMMAND avconv -i video.mkv -s 1280x800 -c:v libx264 smallVideo.mkv)

include_directories(lib)
