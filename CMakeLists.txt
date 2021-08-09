cmake_minimum_required(VERSION 3.8)

project(drawGraph CXX)

# Some basics, set accordingly to your needs.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Where the 'app' and 'blend2d' are.
message(${CMAKE_CURRENT_LIST_DIR})
set(APP_DIR "${CMAKE_CURRENT_LIST_DIR}"
    CACHE PATH "Application directory")

set(BLEND2D_DIR "${APP_DIR}/thirdparty/blend2d"
    CACHE PATH "Location of 'blend2d'")
message(${BLEND2D_DIR})

# Enable Blend2D static build.
set(BLEND2D_STATIC TRUE)
include("${BLEND2D_DIR}/CMakeLists.txt")

# Your application target.
add_executable(drawGraph ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp)

# Add Blend2D dependency to your application. The dependency
# should provide everything needed - include paths, libraries,
# compiler flags, and compiler definitions.
target_link_libraries(drawGraph Blend2D::Blend2D)
