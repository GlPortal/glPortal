# - Find TinyXML2
# Find the native TinyXML2 includes and library
#
#   TINYXML2_FOUND       - True if TinyXML found.
#   TINYXML2_INCLUDE_DIR - where to find tinyxml.h, etc.
#   TINYXML2_LIBRARIES   - List of libraries when using TinyXML.
#

IF( TINYXML2_INCLUDE_DIR )
    # Already in cache, be silent
    SET( TinyXML2_FIND_QUIETLY TRUE )
ENDIF( TINYXML2_INCLUDE_DIR )

FIND_PATH( TINYXML2_INCLUDE_DIR "tinyxml2.h"
  PATHS /usr/include
           PATH_SUFFIXES "tinyxml2" )
set (TINYXML2_INCLUDE_DIRS ${TINYXML2_INCLUDE_DIR})

FIND_LIBRARY( TINYXML2_LIBRARY
              NAMES "tinyxml2"
              PATH_SUFFIXES "tinyxml2" )
set (TINYXML2_LIBRARIES ${TINYXML2_LIBRARY})

# handle the QUIETLY and REQUIRED arguments and set TINYXML_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE( "FindPackageHandleStandardArgs" )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( "TinyXML2" DEFAULT_MSG TINYXML2_LIBRARY TINYXML2_INCLUDE_DIR )

MARK_AS_ADVANCED( TINYXML2_LIBRARY TINYXML2_INCLUDE_DIR TINYXML2_INCLUDE_DIRS TINYXML2_LIBRARIES )
