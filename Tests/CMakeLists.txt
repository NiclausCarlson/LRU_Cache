project(Tests)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(LRUCacheTest_run main_tests.cpp LRUCacheTest.cpp ../MyLRUCache/MyLRUCache.h)
target_link_libraries(LRUCacheTest_run gtest gtest_main)