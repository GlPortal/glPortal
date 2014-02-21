  include(CMakeForceCompiler)
  SET(CMAKE_SYSTEM_NAME Windows)

  SET(CMAKE_FORCE_C_COMPILER i686-w64-mingw32-gcc)
  SET(CMAKE_FORCE_CXX_COMPILER i686-w64-mingw32-g++)
  SET(CMAKE_FORCE_RC_COMPILER i686-w64-mingw32-windres)
  SET(CMAKE_RC_COMPILER i686-w64-mingw32-windres)

  SET(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -march=pentium4 -mtune=pentium4 -mwindows -O2")
  SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "-Wl,--no-undefined -static-libgcc -Wl,-O1 -Wl,--as-needed -Wl,--sort-common -s")

  SET(CMAKE_FIND_ROOT_PATH /usr/i686-w64-mingw32)

  SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
  SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
  SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
  include_directories(
  /usr/i686-w64-mingw32/include
  /usr/i686-w64-mingw32
  /usr/lib/gcc/i686-w64-mingw32/4.6/include 
  /usr/lib/gcc/i686-w64-mingw32/4.6
  /usr/i686-w64-mingw32/include
  /usr/i686-w64-mingw32
  )