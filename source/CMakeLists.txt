
include_directories(lib engine ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${EXE_NAME} 
  Game.cpp
  engine/util/Vector3f.cpp
  engine/util/Vector2f.cpp
  engine/util/Matrix4f.cpp
  World.cpp Window.cpp
  engine/Camera.cpp
  engine/TextureLoader.cpp
  engine/Resource.cpp
  Input.cpp engine/Renderer.cpp
  engine/MapLoader.cpp
  engine/environment/Environment.cpp
  engine/environment/ConfigFileParser.cpp
  engine/environment/Path.cpp

#  Main.cpp Game.cpp Path.cpp
#  engine/network/client/UdpClient.cpp
#  engine/gui/GameScreen.cpp engine/Resources.cpp engine/StringConverter.cpp 
#  engine/gui/Character.cpp  engine/gui/Font.cpp engine/tools/Timer.cpp
#  engine/object/Model.cpp engine/object/Mesh.cpp
#  engine/renderer/GameMapRenderer.cpp 
#  map/MapFileParser.cpp
#  util/ListFileParser.cpp
#  util/ConfigFileParser.cpp
#  util/audioplayer/AudioPlayer.cpp
#  util/Vector3f.cpp
#  engine/Environment.cpp
#  Player.cpp GameMap.cpp Light.cpp 
#  Shot.cpp Portal.cpp 
#  Window.cpp Exception.cpp
  )
target_link_libraries(${EXE_NAME} ${PORTAL_LIBRARIES})
install ( TARGETS ${EXE_NAME} DESTINATION bin )

#add_executable(${SERVER_EXE_NAME} EXCLUDE_FROM_ALL
#  engine/network/server/Main.cpp  engine/network/server/UdpServer.cpp
#  engine/network/server/TcpServer.cpp
#  engine/network/server/TcpSession.cpp
#  util/sheduler/Sheduler.cpp util/sheduler/Job.hpp
#  util/html/Generator.cpp     
#  engine/network/server/controllers/StatisticsController.cpp
#  engine/network/server/Controller.cpp
#  )

#target_link_libraries(${SERVER_EXE_NAME} ${PORTAL_LIBRARIES} boost_system boost_thread boost_filesystem)

#add_custom_target(run COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}${CMAKE_EXECUTABLE_SUFFIX} -datadir ${CMAKE_SOURCE_DIR}/data )
#add_custom_target(capture COMMAND avconv -f alsa -f x11grab -r 30 -s 1680x1050 -i :0.0+0,0 -acodec libfaac -vcodec libx264 -pre:0 lossless_ultrafast -threads 0 video.mkv)
#add_custom_target(compressVideo COMMAND avconv -i video.mkv -s 1280x800 -c:v libx264 smallVideo.mkv)
