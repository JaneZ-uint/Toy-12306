cmake_minimum_required(VERSION 3.29)
project(Toy12306)

set(CMAKE_CXX_STANDARD 20)

add_executable(Toy12306 main.cpp
        "Data Structure/BPT.h"
        "Data Structure/vector.h"
        "Data Structure/list.h"
        "Data Structure/LRUCache.h"
        "Data Structure/exceptions.h"
        Management/ticket.cpp
        Management/ticket.h
        Management/train.cpp
        Management/train.h
        Tokenscanner/tokenscanner.h
        Tokenscanner/tokenscanner.cpp
        Management/user.cpp
        Management/user.h)
