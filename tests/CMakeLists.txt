include_directories(lib engine ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../source)
add_executable(xmlReadingTest XmlReadingTest.cpp ../source/util/math/Vector3f.cpp ../source/engine/loader/XmlHelper.cpp)
target_link_libraries(xmlReadingTest UnitTest++ ${PORTAL_TEST_LIBRARIES} tinyxml)
add_test(NAME xmlReadingTest COMMAND xmlReadingTest)
