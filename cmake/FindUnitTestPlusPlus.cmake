#
# This CMake Module locates the UnitTest++ (http://unittest-cpp.sourceforge.net/)
# C++ unit testing framework, enabling FIND_PACKAGE(UnitTestPlusPlus) to work.
#

FIND_PATH(UnitTestPlusPlus_INCLUDE_DIR NAMES unittest++/UnitTest++.h)
MARK_AS_ADVANCED(UnitTestPlusPlus_INCLUDE_DIR)

FIND_LIBRARY(UnitTestPlusPlus_LIBRARY NAMES UnitTest++)
MARK_AS_ADVANCED(UnitTestPlusPlus_LIBRARY)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(UnitTestPlusPlus DEFAULT_MSG UnitTestPlusPlus_LIBRARY UnitTestPlusPlus_INCLUDE_DIR)

IF ("${UnitTestPlusPlus_INCLUDE_DIR}" MATCHES "NOTFOUND")
SET (UnitTestPlusPlus_LIBRARY)
SET (UnitTestPlusPlus_INCLUDE_DIR)
ELSEIF ("${UnitTestPlusPlus_LIBRARY}" MATCHES "NOTFOUND")
SET (UnitTestPlusPlus_LIBRARY)
SET (UnitTestPlusPLus_INCLUDE_DIR)
ELSE ("${UnitTestPlusPlus_INCLUDE_DIR}" MATCHES "NOTFOUND")
SET (UnitTestPlusPlus_FOUND 1)
SET (UnitTestPlusPlus_LIBRARIES ${UnitTestPlusPlus_LIBRARY})
SET (UnitTestPlusPlus_INCLUDE_DIRS ${UnitTestPlusPlus_INCLUDE_DIR})
ENDIF ("${UnitTestPlusPlus_INCLUDE_DIR}" MATCHES "NOTFOUND")