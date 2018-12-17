cmake_minimum_required(VERSION 3.12)
project(22C)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(22C
        BinaryNode.h
        BinarySearchTree.h
        BinaryTree.h
        HashTable.h
        LinkedList.h
        main.cpp
        Tutor.h)
