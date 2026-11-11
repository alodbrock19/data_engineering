cmake_minimum_required(VERSION 3.16)
project(tpch_join CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimized code with debug information: fast, and perf can map samples back to source lines.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

add_executable(tpch_join main.cpp)
find_package(Threads REQUIRED)
target_link_libraries(tpch_join PRIVATE Threads::Threads)
# Frame pointers let `perf record -g` reconstruct call stacks cheaply.
target_compile_options(tpch_join PRIVATE -Wall -Wextra -fno-omit-frame-pointer)
