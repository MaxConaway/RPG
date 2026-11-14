cmake_minimum_required(VERSION 4.4)
project(RPG)

set(CMAKE_CXX_STANDARD 20)

add_executable(RPG Adventure.cpp
        Scene.h
        StoryTree.h
        Adventure.h
        Item.h
        List.h
        main.cpp
        Moment.h
        Node.h
)
