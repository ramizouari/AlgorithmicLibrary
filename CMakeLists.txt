cmake_minimum_required(VERSION 3.20)
project(AlgorithmicLibrary)

set(CMAKE_CXX_STANDARD 23)

include_directories(graph)
include_directories(struct)
file(GLOB algo_files CONFIGURE_DEPENDS "*.h" "struct/*.h" "graph/*.h" "comptability/*.h")
add_executable(AlgorithmicLibrary
        ${algo_files}
        main_2.cpp)

add_executable(SegmentTreeTest
        ${algo_files}
        test/segment_tree_test.cpp)

