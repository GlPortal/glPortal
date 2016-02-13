cmake_minimum_required(VERSION 2.8)
set(EXE_NAME glportal)

if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type")
endif()

project(${EXE_NAME})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if(DEFINED WIN32)
  add_definitions("-D_USE_MATH_DEFINES")
endif()

find_package(CXX14 REQUIRED)
set(CMAKE_CXX_FLAGS "${CXX14_FLAGS} ${CMAKE_CXX_FLAGS}")

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
# Don't load libGLU, that's old GL1.0 crap.
set(PORTAL_LIBRARIES ${OPENGL_gl_LIBRARY})

find_package(PkgConfig REQUIRED)
pkg_search_module(EPOXY REQUIRED epoxy)
find_package(UnitTestPlusPlus)
include_directories(${EPOXY_INCLUDE_DIRS})
list(APPEND PORTAL_LIBRARIES ${EPOXY_LIBRARIES} )

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

find_package(GWEN REQUIRED)
include_directories(${GWEN_INCLUDE_DIR})
list(APPEND PORTAL_LIBRARIES ${GWEN_LIBRARY})

find_package(Bullet 2.82 REQUIRED)
include_directories(${BULLET_INCLUDE_DIR})
list(APPEND PORTAL_LIBRARIES ${BULLET_LIBRARIES})

find_package(VHACD REQUIRED)
include_directories(${VHACD_INCLUDE_DIR})
list(APPEND PORTAL_LIBRARIES ${VHACD_LIBRARY})

set(PORTAL_TEST_LIBRARIES ${PORTAL_LIBRARIES})

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  add_definitions("-Wall")
endif ()

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
