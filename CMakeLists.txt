cmake_minimum_required(VERSION 2.8)
set(EXE_NAME glportal)

if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type")
endif()

project(${EXE_NAME})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#### PLATFORM-SPECIFIC FIXES ####

if(DEFINED WIN32)
  if(MSVC)
    add_definitions("-DNOMINMAX")
  endif()
  if(MSVC)
    add_definitions("-Dand=&&")
    add_definitions("-Dor=||")
    add_definitions("-Dnot=!")
  endif()
endif()

# Sort of dirty hack, try to fix its
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/RadixEngine/cmake)

set(PORTAL_LIBRARIES "")

find_package(CXX14 REQUIRED)
set(CMAKE_CXX_FLAGS "${CXX14_FLAGS} ${CMAKE_CXX_FLAGS}")

find_package(RadixEngine REQUIRED)
include_directories(${RADIX_INCLUDE_DIRS})
list(APPEND PORTAL_LIBRARIES ${RADIX_LIBRARIES})

if (
  (NOT DEFINED EPOXY_SUPPORT) AND # Epoxy
  (NOT DEFINED GLAD_SUPPORT)      # GLAD
)
  if(WIN32)
    set(GLAD_SUPPORT TRUE)
  else()
    set(EPOXY_SUPPORT TRUE)
  endif()
endif()

if (EPOXY_SUPPORT)
  add_definitions("-DEPOXY_SUPPORT")
elseif (GLAD_SUPPORT) 
  add_definitions("-DGLAD_SUPPORT")
endif ()

set(PORTAL_TEST_LIBRARIES ${PORTAL_LIBRARIES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

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

if (MSVC)
  add_custom_target(versionInfo ALL 
    COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR} & call ./scripts/writeVer.bat ${CMAKE_CURRENT_SOURCE_DIR}/source/Version.hpp
  )
elseif(APPLE)
  add_custom_target(versionInfo ALL
    COMMAND echo \"\#define GAME_VERSION \\\"\\c\">${CMAKE_CURRENT_SOURCE_DIR}/source/Version.hpp
    COMMAND git describe | tr -d \"\\n\" >>${CMAKE_CURRENT_SOURCE_DIR}/source/Version.hpp
    COMMAND echo \"\\\"\\c\">>${CMAKE_CURRENT_SOURCE_DIR}/source/Version.hpp
    COMMAND git update-index --assume-unchanged ${CMAKE_CURRENT_SOURCE_DIR}/source/Version.hpp
  )
else ()
  add_custom_target(versionInfo ALL
    COMMAND echo -n \"\#define GAME_VERSION \\\"\">${CMAKE_CURRENT_SOURCE_DIR}/source/Version.hpp
    COMMAND git describe | tr -d \"\\n\" >>${CMAKE_CURRENT_SOURCE_DIR}/source/Version.hpp
    COMMAND echo -n \"\\\"\">>${CMAKE_CURRENT_SOURCE_DIR}/source/Version.hpp
    COMMAND git update-index --assume-unchanged ${CMAKE_CURRENT_SOURCE_DIR}/source/Version.hpp
  )
endif ()

add_dependencies(${EXE_NAME} versionInfo)
