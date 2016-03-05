set(RADIX_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/RadixEngine")
# set(RADIX_INCLUDE_DIR "${RADIX_ROOT_DIR}/include")
# set(RADIX_INCLUDE_DIRS "${RADIX_INCLUDE_DIR}" "${RADIX_ROOT_DIR}/external/serine/include")
# set(RADIX_LIBRARY "RadixEngine")
# set(RADIX_LIBRARIES ${RADIX_LIBRARY})

add_subdirectory("${RADIX_ROOT_DIR}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RADIX DEFAULT_MSG RADIX_LIBRARIES RADIX_INCLUDE_DIR RADIX_INCLUDE_DIRS)

mark_as_advanced(RADIX_LIBRARY RADIX_LIBRARIES RADIX_INCLUDE_DIR RADIX_INCLUDE_DIRS)
