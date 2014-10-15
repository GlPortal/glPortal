include_directories(lib engine ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${EXE_NAME} 
  Game.cpp
  Portal.cpp
  engine/Ray.cpp    
  util/math/Vector3f.cpp
  util/math/Vector2f.cpp
  util/math/Matrix4f.cpp
  util/math/Math.cpp
  util/logger/Logger.cpp
  util/logger/ConsoleLogger.cpp
  util/sdl/Fps.cpp
  engine/BoxCollider.cpp
  World.cpp Window.cpp
  Player.cpp
  engine/Camera.cpp
  Input.cpp engine/renderer/Renderer.cpp
  engine/Text.cpp
  engine/loader/MapLoader.cpp
  engine/loader/MeshLoader.cpp
  engine/loader/ShaderLoader.cpp
  engine/loader/TextureLoader.cpp
  engine/loader/FontLoader.cpp
  engine/loader/XmlHelper.cpp
  engine/env/Environment.cpp
  engine/env/System.cpp
  engine/env/ConfigFileParser.cpp
  engine/env/Config.cpp
  engine/env/Path.cpp
  )
target_link_libraries(${EXE_NAME} ${PORTAL_LIBRARIES} tinyxml)
install(TARGETS ${EXE_NAME} DESTINATION bin)

add_custom_target(run COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}${CMAKE_EXECUTABLE_SUFFIX} -datadir ${CMAKE_SOURCE_DIR}/data )
add_custom_target(capture COMMAND avconv -f alsa -f x11grab -r 30 -s 1680x1050 -i :0.0+0,0 -acodec libfaac -vcodec libx264 -pre:0 lossless_ultrafast -threads 0 video.mkv)
add_custom_target(compressVideo COMMAND avconv -i video.mkv -s 1280x800 -c:v libx264 smallVideo.mkv)
