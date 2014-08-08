
include_directories(lib engine ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${EXE_NAME} 
  Game.cpp
  Portal.cpp
  engine/Ray.cpp    
  engine/util/Vector3f.cpp
  engine/util/Vector2f.cpp
  engine/util/Matrix4f.cpp
  engine/util/Math.cpp
  engine/BoxCollider.cpp
  World.cpp Window.cpp
  engine/Camera.cpp
  engine/TextureLoader.cpp
  engine/Resource.cpp
  Input.cpp engine/Renderer.cpp
  engine/MapLoader.cpp
  engine/MeshLoader.cpp
  engine/environment/Environment.cpp
  engine/environment/ConfigFileParser.cpp
  engine/environment/Path.cpp
  )
target_link_libraries(${EXE_NAME} ${PORTAL_LIBRARIES} tinyxml)
install ( TARGETS ${EXE_NAME} DESTINATION bin )

add_custom_target(run COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}${CMAKE_EXECUTABLE_SUFFIX} -datadir ${CMAKE_SOURCE_DIR}/data )
add_custom_target(capture COMMAND avconv -f alsa -f x11grab -r 30 -s 1680x1050 -i :0.0+0,0 -acodec libfaac -vcodec libx264 -pre:0 lossless_ultrafast -threads 0 video.mkv)
add_custom_target(compressVideo COMMAND avconv -i video.mkv -s 1280x800 -c:v libx264 smallVideo.mkv)
