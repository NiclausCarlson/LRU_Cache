cmake_minimum_required(VERSION 3.20)
project(LRU_Cache)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Tests)
add_executable(LRU_Cache MyLRUCache/MyLRUCache.h main.cpp)
