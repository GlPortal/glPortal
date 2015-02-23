include_directories(lib engine ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${EXE_NAME} 
  Game.cpp
  Portal.cpp
  engine/Ray.cpp
  engine/SoundManager.cpp    
  engine/core/math/Vector3f.cpp
  engine/core/math/Vector2f.cpp
  engine/core/math/Matrix4f.cpp
  engine/core/math/Math.cpp
  engine/core/diag/Logger.cpp
  engine/core/diag/ConsoleLogger.cpp
  util/sdl/Fps.cpp
  engine/BoxCollider.cpp
  World.cpp Window.cpp
  Player.cpp
  engine/trigger/Trigger.cpp
  engine/Camera.cpp
  Input.cpp engine/renderer/Renderer.cpp
  assets/map/MapLoader.cpp
  assets/map/MapListLoader.cpp
  assets/map/XmlHelper.cpp
  assets/model/MeshLoader.cpp
  assets/shader/ShaderLoader.cpp
  assets/texture/TextureLoader.cpp
  assets/text/FontLoader.cpp
  assets/gui/GUIButton.cpp
  engine/env/Environment.cpp
  engine/env/System.cpp
  engine/env/ConfigFileParser.cpp
  engine/env/Config.cpp
  engine/core/file/Path.cpp
  )
target_link_libraries(${EXE_NAME} ${PORTAL_LIBRARIES})
install(TARGETS ${EXE_NAME} DESTINATION bin)

add_custom_target(run COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}${CMAKE_EXECUTABLE_SUFFIX} -datadir ${CMAKE_SOURCE_DIR}/data )
add_custom_target(capture COMMAND avconv -f alsa -f x11grab -r 30 -s 1680x1050 -i :0.0+0,0 -acodec libfaac -vcodec libx264 -pre:0 lossless_ultrafast -threads 0 video.mkv)
add_custom_target(compressVideo COMMAND avconv -i video.mkv -s 1280x800 -c:v libx264 smallVideo.mkv)
