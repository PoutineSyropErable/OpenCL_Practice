@echo off

call check_and_install_both.bat
echo.

if %errorlevel% neq 0 (
    echo [ERROR] Failed to set up vcpkg and OpenCL. Exiting...
    exit /b 1
)

:: Set paths
set BUILD_DIR=build
set EXECUTABLE=VectorAdditionOnGraphicsCard.exe

:: Check if the build directory exists; if not, create it
if not exist %BUILD_DIR% mkdir %BUILD_DIR%

echo [INFO] Creating the Makefile using From CMake Config file.
echo.

:: Run CMake to configure the project
cmake -S . -B %BUILD_DIR% || (
    echo.
    echo [ERROR] CMake configuration failed!
    exit /b 1
)


echo [INFO] Building the Project
echo.

:: Build the project
cmake --build %BUILD_DIR% || (
    echo.
    echo [ERROR] Build failed!
    exit /b 1
)

echo [INFO] Project Build Sucessfull, Trying to find the executable
echo.

:: Check if the executable exists
if not exist %BUILD_DIR%\%EXECUTABLE% (
    echo.
    echo [ERROR] Build succeeded but executable not found in %BUILD_DIR%!
    echo [ERROR] Please check your CMake configuration and paths.
    exit /b 1
)

:: Run the executable
echo.
echo [INFO] Executable Found. Running the program...
echo.
%BUILD_DIR%\%EXECUTABLE% || (
    echo.
    echo [ERROR] Program execution failed!
    exit /b 1
)

:: Success message
echo.
echo [SUCCESS] Program executed successfully!
echo.
