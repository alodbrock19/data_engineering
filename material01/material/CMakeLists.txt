cmake_minimum_required(VERSION 3.16)
project(tinydb CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/table.cpp
)
set(HEADERS
    include/lineitem.hpp
    include/scoped_timer.hpp
    include/table.hpp
)

add_executable(tinydb ${SOURCES} ${HEADERS})
target_include_directories(tinydb PRIVATE include)
