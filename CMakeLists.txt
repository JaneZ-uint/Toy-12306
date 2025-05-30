cmake_minimum_required(VERSION 3.29)
project(Toy12306)

set(CMAKE_CXX_STANDARD 20)

add_executable(Toy12306 main.cpp
        include/exceptions.h
        include/list.h
        include/map.h
        include/priority_queue.h
        include/String.h
        include/utility.h
        include/ticket.h
        include/time.h
        include/time.h
        include/HashFunction.h
        include/MemoryRiver.h
        include/tokenscanner.h
        include/tokenSlicer.h
        src/ticket.cpp
        src/time.cpp
        src/train.cpp
        src/user.cpp
        src/tokenscanner.cpp
        src/tokenSlicer.cpp
)
