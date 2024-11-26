cmake_minimum_required(VERSION 3.10)

project(HelloOpenCL LANGUAGES C)

# Include the OpenCL headers
include_directories(E:/School and Work/University (real)/Semester 9/Personal/Run code GPU/vcpkg/installed/x64-windows/include)

# Link the OpenCL library
link_directories(E:/School and Work/University (real)/Semester 9/Personal/Run code GPU/vcpkg/installed/x64-windows/lib)

add_executable(${PROJECT_NAME} main.c)

# Link the OpenCL library to the executable
target_link_libraries(${PROJECT_NAME} PRIVATE OpenCL)
