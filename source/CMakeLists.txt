include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(NOT DEFINED PORTAL_LIBRARIES)
  message(FATAL_ERROR "Please compile using the repo's root CMakeLists.txt")
endif()

add_executable(${EXE_NAME}
  assets/map/MapListLoader.cpp
  assets/map/MapLoader.cpp
  assets/map/XmlHelper.cpp
  assets/material/MaterialLoader.cpp
  assets/model/MeshLoader.cpp
  assets/scene/Scene.cpp
  assets/scene/SceneHelper.cpp
  assets/scene/Screen.cpp
  assets/scene/Terminal.cpp
  assets/shader/Shader.cpp
  assets/shader/ShaderLoader.cpp
  assets/text/FontLoader.cpp
  assets/texture/TextureLoader.cpp
  engine/Camera.cpp
  engine/Camera.cpp
  engine/component/Health.cpp
  engine/component/Player.cpp
  engine/component/RigidBody.cpp
  engine/component/SoundSource.cpp
  engine/component/Transform.cpp
  engine/core/diag/AnsiConsoleLogger.cpp
  engine/core/diag/Logger.cpp
  engine/core/diag/LogInput.cpp
  engine/core/diag/StdoutLogger.cpp
  engine/core/event/Dispatcher.cpp
  engine/core/event/Observer.cpp
  engine/core/event/observer/FlagObserver.cpp
  engine/core/event/observer/MusicObserver.cpp
  engine/core/file/Path.cpp
  engine/core/gl/TightDataPacker.cpp
  engine/core/gl/VBO.cpp
  engine/core/math/Math.cpp
  engine/core/math/Matrix3f.cpp
  engine/core/math/Matrix4f.cpp
  engine/core/math/Vector2f.cpp
  engine/core/math/Vector3f.cpp
  engine/core/math/Vector4f.cpp
  engine/core/state/GameState.cpp
  engine/core/state/PlayerState.cpp
  engine/EntityManager.cpp
  engine/env/ArgumentsParser.cpp
  engine/env/Config.cpp
  engine/env/Environment.cpp
  engine/env/System.cpp
  engine/physics/CollisionDispatcher.cpp
  engine/physics/KinematicCharacterController.cpp
  engine/physics/PhysicsDebugDraw.cpp
  engine/physics/PhysicsHelper.cpp
  engine/physics/Uncollider.cpp
  engine/renderer/GWENRenderer.cpp
  engine/renderer/Renderer.cpp
  engine/renderer/TerminalRenderer.cpp
  engine/renderer/UiRenderer.cpp
  engine/SoundManager.cpp
  engine/system/PhysicsSystem.cpp
  engine/system/PlayerSystem.cpp
  engine/system/PortalSystem.cpp
  util/sdl/Fps.cpp
  Editor.cpp
  GameController.cpp
  Game.cpp
  Input.cpp
  Portal.cpp
  Window.cpp
  World.cpp
  WorldHelper.cpp
)
target_link_libraries(${EXE_NAME} ${PORTAL_LIBRARIES})
install(TARGETS ${EXE_NAME} DESTINATION bin)
add_custom_target(update_assets COMMAND git submodule update --depth 1)
add_custom_target(run COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}${CMAKE_EXECUTABLE_SUFFIX} --datadir ${CMAKE_SOURCE_DIR}/data DEPENDS ${EXE_NAME})
add_custom_target(maptest COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}${CMAKE_EXECUTABLE_SUFFIX} --datadir ${CMAKE_SOURCE_DIR}/data --map test DEPENDS ${EXE_NAME} update_assets)
add_custom_target(capture COMMAND avconv -f alsa -f x11grab -r 30 -s 1680x1050 -i :0.0+0,0 -acodec libfaac -vcodec libx264 -pre:0 lossless_ultrafast -threads 0 video.mkv)
add_custom_target(compressVideo COMMAND avconv -i video.mkv -s 1280x800 -c:v libx264 smallVideo.mkv)
#depends on package similarity-tester
add_custom_target(cpd COMMAND sim_c -f -F ./*.cpp)
