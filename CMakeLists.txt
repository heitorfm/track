cmake_minimum_required(VERSION 3.28)
project(track)

#add_subdirectory(src)

set(CMAKE_CXX_STANDARD 20)


add_executable(track src/main.cpp
    src/Tracker.cpp
    src/Reporter.cpp
)

find_package(fmt)
target_link_libraries(track fmt::fmt)
