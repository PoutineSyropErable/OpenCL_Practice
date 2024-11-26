@echo off

:: Set paths
set BUILD_DIR=build
set EXECUTABLE=VectorAdditionOnGraphicsCard.exe

:: Check if the build directory exists; if not, create it
if not exist %BUILD_DIR% mkdir %BUILD_DIR%

:: Run CMake to configure the project
cmake -S . -B %BUILD_DIR% || (
    echo.
    echo CMake configuration failed!
    exit /b 1
)

:: Build the project
cmake --build %BUILD_DIR% || (
    echo.
    echo Build failed!
    exit /b 1
)

:: Check if the executable exists
if not exist %BUILD_DIR%\%EXECUTABLE% (
    echo.
    echo Build succeeded but executable not found in %BUILD_DIR%!
    echo Please check your CMake configuration and paths.
    exit /b 1
)

:: Run the executable
echo.
echo Running the program...
echo.
%BUILD_DIR%\%EXECUTABLE% || (
    echo.
    echo Program execution failed!
    exit /b 1
)

:: Success message
echo.
echo Program executed successfully!
