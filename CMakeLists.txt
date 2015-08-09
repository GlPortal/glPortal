cmake_minimum_required(VERSION 2.8)
set(EXE_NAME glportal)

if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type")
endif()

project(${EXE_NAME})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


find_package(CXX11 REQUIRED)
set(CMAKE_CXX_FLAGS "${CXX11_FLAGS} ${CMAKE_CXX_FLAGS}")

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(EPOXY REQUIRED epoxy)
find_package(UnitTestPlusPlus)
include_directories(${OPENGL_INCLUDE_DIR} ${EPOXY_INCLUDE_DIRS})
set(PORTAL_LIBRARIES ${OPENGL_LIBRARIES} ${EPOXY_LIBRARIES} )

find_package(Assimp REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIRS})
list(APPEND PORTAL_LIBRARIES ${ASSIMP_LIBRARIES} )

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIRS})
list(APPEND PORTAL_LIBRARIES ${SDL2_LIBRARY} ${SDL2_MIXER_LIBRARIES} )

find_package(TinyXML2 REQUIRED)
include_directories(${TINYXML2_INCLUDE_DIR})
list(APPEND PORTAL_LIBRARIES ${TINYXML2_LIBRARIES} )

find_package(json11 REQUIRED)
include_directories(${JSON11_INCLUDE_DIR})
list(APPEND PORTAL_LIBRARIES ${JSON11_LIBRARY})

find_package(Bullet 2.82 REQUIRED)
include_directories(${BULLET_INCLUDE_DIR})
list(APPEND PORTAL_LIBRARIES ${BULLET_LIBRARIES})

set(PORTAL_TEST_LIBRARIES ${PORTAL_LIBRARIES})

add_subdirectory(data)
add_subdirectory(source)

if (UnitTestPlusPlus_FOUND)
  enable_testing()
  add_subdirectory(tests)
endif ()

if (CMAKE_VERSION VERSION_GREATER 2.8.5)
  include(FeatureSummary)
  feature_summary(WHAT ALL)
endif ()
