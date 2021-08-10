cmake_minimum_required(VERSION 3.8)

project(drawGraph CXX)

# Some basics, set accordingly to your needs.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES
        src/*.cpp
        src/*.cc
        src/*.c)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

# Where the 'app' and 'blend2d' are.
message(${CMAKE_CURRENT_LIST_DIR})
set(APP_DIR "${CMAKE_CURRENT_LIST_DIR}"
    CACHE PATH "Application directory")

set(BLEND2D_DIR "${APP_DIR}/thirdparty/blend2d"
    CACHE PATH "Location of 'blend2d'")
message(${BLEND2D_DIR})

set(EIGEN_DIR "${APP_DIR}/thirdparty/eigen"
    CACHE PATH "Location of 'eigen'")

# Enable Blend2D static build.
set(BLEND2D_STATIC TRUE)
include("${BLEND2D_DIR}/CMakeLists.txt")

include_directories("${CMAKE_CURRENT_LIST_DIR}/src")

message(${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CMAKE_CURRENT_LIST_DIR}/thirdparty/eigen)

# Your application target.
add_executable(test ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp)
target_include_directories(test SYSTEM PUBLIC ${CMAKE_CURRENT_LIST_DIR}/thirdparty/eigen)

# Add Blend2D dependency to your application. The dependency
# should provide everything needed - include paths, libraries,
# compiler flags, and compiler definitions.
target_link_libraries(drawGraph Blend2D::Blend2D)
target_link_libraries(test Blend2D::Blend2D)
target_link_libraries(test ${PROJECT_NAME})
