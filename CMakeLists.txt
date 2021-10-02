cmake_minimum_required(VERSION 3.20)
project(AlgorithmicLibrary)

set(CMAKE_CXX_STANDARD 23)

include_directories(graph)
include_directories(struct)

add_executable(AlgorithmicLibrary
        graph/abstract_graph.h
        graph/abstract_weighted_graph.h
        graph/directed_graph.h
        graph/directed_weighted_graph.h
        graph/graph_functions.h
        graph/graph_utils.h
        graph/undirected_graph.h
        graph/undirected_weighted_graph.h
        struct/binary_search_tree.h
        struct/disjoint_set.h
        struct/dynamic_array.h
        struct/forward_list.h
        struct/hash.h
        struct/hashmap.h
        struct/hashset.h
        struct/heap.h
        struct/list.h
        struct/queue.h
        struct/binary_operator.h
        struct/ordered_segment_tree.h
        struct/dynamic_segment_tree.h
        struct/sparse_array.h
        struct/segment_tree.h
        struct/stack.h
        main_2.cpp)
