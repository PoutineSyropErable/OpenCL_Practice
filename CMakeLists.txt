cmake_minimum_required(VERSION 3.10)
project(VectorAdditionOnGraphicsCard)

# Specify C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories for OpenCL
include_directories(${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows/include)

# Link directories for OpenCL
link_directories(${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows/lib)

# Source files
set(SOURCES main.c)

# Ensure executable is placed in the project root build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Add executable target
add_executable(VectorAdditionOnGraphicsCard ${SOURCES})

# Link OpenCL library
target_link_libraries(VectorAdditionOnGraphicsCard OpenCL)
