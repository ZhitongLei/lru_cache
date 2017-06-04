include_directories(
    ../src
    ../3rd/google-benchmark/include
)

link_directories(
    ../3rd/google-benchmark/lib
)

set(DIR_CACHE_TEST_SRCS cache_test.cpp)
message(STATUS ${DIR_CACHE_TEST_SRCS})
add_executable(cache_test ${DIR_CACHE_TEST_SRCS})
target_link_libraries(cache_test lru_cache pthread)

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wl,--whole-archive -lpthread -Wl,--no-whole-archive")
set(DIR_CACHE_BENCHMARK_SRCS cache_benchmark.cpp)
message(STATUS ${DIR_CACHE_BENCHMARK_SRCS})
add_executable(cache_benchmark ${DIR_CACHE_BENCHMARK_SRCS})
target_link_libraries(cache_benchmark lru_cache libbenchmark.a rt pthread)

#https://www.ibm.com/developerworks/cn/linux/l-cn-cmake/index.html
#answer of tuvalu -- http://stackoverflow.com/questions/8649828/what-is-the-correct-link-options-to-use-stdthread-in-gcc-under-linux 
