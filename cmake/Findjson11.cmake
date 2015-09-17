set(JSON11_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")
set(JSON11_INCLUDE_DIRS ${JSON11_INCLUDE_DIR})
set(JSON11_LIBRARY "json11")
set(JSON11_LIBRARIES ${JSON11_LIBRARY})

add_subdirectory("${JSON11_INCLUDE_DIR}/json11")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(json11 DEFAULT_MSG JSON11_LIBRARIES JSON11_INCLUDE_DIR JSON11_INCLUDE_DIRS)

mark_as_advanced(JSON11_LIBRARY JSON11_LIBRARIES JSON11_INCLUDE_DIR JSON11_INCLUDE_DIRS)
