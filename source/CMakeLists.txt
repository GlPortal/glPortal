include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(NOT DEFINED PORTAL_LIBRARIES)
  message(FATAL_ERROR "Please compile using the repo's root CMakeLists.txt")
endif()

add_executable(${EXE_NAME}
  renderer/TerminalRenderer.cpp
  renderer/UiRenderer.cpp
  renderer/GameRenderer.cpp
        trigger/PortalTeleport.cpp
  Editor.cpp
  Main.cpp
  Game.cpp
  GameController.cpp
  GameState.cpp
  Portal.cpp
  World.cpp
  WorldHelper.cpp
)

target_compile_definitions(${EXE_NAME} PUBLIC ${RADIX_DEFINITIONS})
target_link_libraries(${EXE_NAME} ${PORTAL_LIBRARIES})
install(TARGETS ${EXE_NAME} DESTINATION bin)
add_custom_target(update_assets COMMAND git submodule update --depth 1)
add_custom_target(run COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}${CMAKE_EXECUTABLE_SUFFIX} --datadir ${CMAKE_SOURCE_DIR}/data DEPENDS ${EXE_NAME})
add_custom_target(debug COMMAND gdb --args ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}${CMAKE_EXECUTABLE_SUFFIX} --datadir ${CMAKE_SOURCE_DIR}/data DEPENDS ${EXE_NAME})
add_custom_target(maptest COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}${CMAKE_EXECUTABLE_SUFFIX} --datadir ${CMAKE_SOURCE_DIR}/data --map test DEPENDS ${EXE_NAME} update_assets)
add_custom_target(capture COMMAND avconv -f alsa -f x11grab -r 30 -s 1680x1050 -i :0.0+0,0 -acodec libfaac -vcodec libx264 -pre:0 lossless_ultrafast -threads 0 video.mkv)
add_custom_target(compressVideo COMMAND avconv -i video.mkv -s 1280x800 -c:v libx264 smallVideo.mkv)
#depends on package similarity-tester
add_custom_target(cpd COMMAND sim_c -f -F ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
