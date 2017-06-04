set(CMAKE_C_COMPILER "/usr/local/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++")

project(lru_cache)
cmake_minimum_required(VERSION 3.6)

INCLUDE_DIRECTORIES(
    ./
    ./3rd/Murmur3
)

LINK_DIRECTORIES(
    /usr/local/lib
    /usr/lib
    /usr/lib64
)
 
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libgcc -static-libstdc++ -static")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libgcc -static-libstdc++")
#set(CMAKE_BUILD_TYPE Debug)
# we can create Makefile for debug or release: cmake -DCMAKE_BUILD_TYPE=Release ${project-path}
#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall")

add_subdirectory(./3rd/Murmur3)
aux_source_directory(./src SRC_LIST)
add_library(lru_cache ${SRC_LIST})
target_link_libraries(lru_cache murmur3)

#add_subdirectory(./3rd/google-benchmark)
add_subdirectory(./example)
