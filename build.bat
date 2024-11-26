@echo off

echo.

:: Set paths to OpenCL headers and libraries (quote paths to handle spaces)
set INCLUDE_PATH=".\vcpkg\installed\x64-windows\include"
set LIB_PATH=".\vcpkg\installed\x64-windows\lib"

:: Set output directory for object files and executable
set BUILD_DIR=bat_build

:: Set output executable name
set OUTPUT=%BUILD_DIR%\VectorAdditionOnGraphicsCard.exe

:: Create the build directory if it doesn't exist
if not exist %BUILD_DIR% mkdir %BUILD_DIR%

:: Clean previous build
if exist %OUTPUT% del %OUTPUT%
if exist %BUILD_DIR%\*.obj del %BUILD_DIR%\*.obj

:: Compile and link (quote paths to avoid issues)
cl /nologo /I%INCLUDE_PATH% main.c /Fo%BUILD_DIR%\ /Fe%OUTPUT% /link /LIBPATH:%LIB_PATH% OpenCL.lib

:: Check if the build succeeded
if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

:: Success message
echo Build succeeded! Executable: %OUTPUT%
echo.

:: Run the program
echo Running the program...
echo.
%OUTPUT%
if errorlevel 1 (
    echo Program execution failed.
    exit /b 1
)

:: Success message
echo.
echo Program executed successfully!
