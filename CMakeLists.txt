# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(enc)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files in the src directory
file(GLOB SOURCES src/*.cpp)

# Platform-specific settings
if(WIN32)
    # Define Windows-specific compiler options or includes
    add_definitions(-D_WIN32)
endif()

# Add the executable
add_executable(enc ${SOURCES})

# Installation rule
install(TARGETS enc DESTINATION bin)
