# - Finds if the compiler has C++11 support
# This module can be used to detect compiler flags for using C++11, and checks
# a small subset of the language.
#
# The following variables are set:
#   CXX14_FLAGS - flags to add to the CXX compiler for C++11 support
#   CXX14_FOUND - true if the compiler supports C++11
#
# TODO: When compilers starts implementing the whole C++11, check the full set

include(CheckCXXSourceCompiles)
include(FindPackageHandleStandardArgs)

set(CXX14_FLAG_CANDIDATES
    #Gnu and Intel Linux
    "-std=c++14"
    #Microsoft Visual Studio, and everything that automatically accepts C++11
    " "
    #Intel windows
    "/Qstd=c++14"
    )

# From C++14 onwards, you can use "if" statements in constexpr functions,
# check for that.
set(CXX14_TEST_SOURCE
"
constexpr inline float sign(float v) {
  if (v > 0.f) return 1.f;
  if (v < 0.f) return -1.f;
  return 0.f;
}
int main() { return sign(0); }
")

foreach(FLAG ${CXX14_FLAG_CANDIDATES})
    set(SAFE_CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS}")
    set(CMAKE_REQUIRED_FLAGS "${FLAG}")
    unset(CXX14_FLAG_DETECTED CACHE)
    message(STATUS "Try C++14 flag = [${FLAG}]")
    check_cxx_source_compiles("${CXX14_TEST_SOURCE}" CXX14_FLAG_DETECTED)
    set(CMAKE_REQUIRED_FLAGS "${SAFE_CMAKE_REQUIRED_FLAGS}")
    if(CXX14_FLAG_DETECTED)
        set(CXX14_FLAGS_INTERNAL "${FLAG}")
        break()
    endif(CXX14_FLAG_DETECTED)
endforeach(FLAG ${CXX14_FLAG_CANDIDATES})

set(CXX14_FLAGS "${CXX14_FLAGS_INTERNAL}")

find_package_handle_standard_args(CXX14 DEFAULT_MSG CXX14_FLAGS)
mark_as_advanced(CXX14_FLAGS)