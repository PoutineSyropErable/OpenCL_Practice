@echo off

:: Step 1: Check and install dependencies
echo [INFO] Checking and installing dependencies...
echo.
call check_and_install_both.bat
if %errorlevel% neq 0 (
    echo [ERROR] Failed to set up vcpkg and OpenCL. Exiting...
    echo.
    exit /b 1
)
echo [SUCCESS] Dependencies are set up.
echo.

:: Step 2: Define paths and variables
set INCLUDE_PATH=".\vcpkg\installed\x64-windows\include"
set LIB_PATH=".\vcpkg\installed\x64-windows\lib"
set BUILD_DIR=bat_build
set OUTPUT=%BUILD_DIR%\VectorAdditionOnGraphicsCard.exe

:: Step 3: Create build directory if it doesn't exist
if not exist %BUILD_DIR% (
    echo [INFO] Creating build directory: %BUILD_DIR%
    echo.
    mkdir %BUILD_DIR%
) else (
    echo [INFO] Build directory already exists: %BUILD_DIR%
    echo.
)
echo.

:: Step 4: Clean previous build
echo [INFO] Cleaning previous build...
echo.
if exist %OUTPUT% del %OUTPUT%
if exist %BUILD_DIR%\*.obj del %BUILD_DIR%\*.obj
echo [SUCCESS] Previous build cleaned.
echo.

:: Step 5: Compile and link
echo [INFO] Compiling and linking the project...
echo.
cl /nologo /I%INCLUDE_PATH% main.c /Fo%BUILD_DIR%\ /Fe%OUTPUT% /link /LIBPATH:%LIB_PATH% OpenCL.lib
if %errorlevel% neq 0 (
    echo [ERROR] Build failed during compilation and linking!
    echo.
    exit /b 1
)
echo [SUCCESS] Build succeeded! Executable: %OUTPUT%
echo.

:: Step 6: Check for executable
if not exist %OUTPUT% (
    echo [ERROR] Build succeeded but executable not found in %BUILD_DIR%!
    echo.
    exit /b 1
)
echo [INFO] Executable found: %OUTPUT%
echo.

:: Step 7: Run the program
echo [INFO] Running the program...
echo.
%OUTPUT%
if %errorlevel% neq 0 (
    echo [ERROR] Program execution failed!
    echo.
    exit /b 1
)
echo [SUCCESS] Program executed successfully!
echo.
