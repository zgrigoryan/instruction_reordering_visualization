cmake_minimum_required(VERSION 3.10)

# Project name and version
project(VolatileVsOptimizedTiming VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(performance_test main.cpp)

# Include Kaizen header-only library (assumes it's in the same folder or in a folder called "kaizen")
target_include_directories(performance_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
