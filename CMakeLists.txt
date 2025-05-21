cmake_minimum_required(VERSION 3.29)
project(Toy12306)

set(CMAKE_CXX_STANDARD 20)

add_executable(code main.cpp
        "Data Structure/BPT.h"
        "Data Structure/vector.h"
        "Data Structure/list.h"
        "Data Structure/LRUCache.h"
        "Data Structure/exceptions.h"
        Management/ticket.cpp
        Management/ticket.h
        Management/train.cpp
        Management/train.h
        Tool/tokenscanner.h
        Tool/tokenscanner.cpp
        Management/user.cpp
        Management/user.h
        "Data Structure/String.h"
        Management/time.h
        Management/time.cpp
        Tool/HashFunction.h
        "Data Structure/map.h"
        Tool/tokenSlicer.h
        Tool/tokenSlicer.cpp
        "Data Structure/utility.h"
        Tool/MemoryRiver.h
        "Data Structure/priority_queue.h"
)
