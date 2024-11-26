@echo off

:: Define paths
set VCPKG_DIR=.\vcpkg
set VCPKG_CMD=%VCPKG_DIR%\vcpkg.exe

:: Define OpenCL package name
set PACKAGE_NAME=opencl:x64-windows

:: Check if vcpkg exists
if not exist %VCPKG_CMD% (
    echo [ERROR] vcpkg is not found in %VCPKG_DIR%.
    echo [ERROR] Please ensure vcpkg is correctly installed and try again.
	echo.
    exit /b 1
)

:: Check if OpenCL is already installed
echo [INFO] Checking if OpenCL is installed...
echo.
%VCPKG_CMD% list | findstr /i /c:"%PACKAGE_NAME%" >nul
if %errorlevel%==0 (
    echo [ERROR] OpenCL is already installed.
	echo.
    exit /b 0
)

:: Install OpenCL
echo [INFO] OpenCL is not installed. Installing now...
%VCPKG_CMD% install %PACKAGE_NAME%
if %errorlevel% neq 0 (
    echo [ERROR] Failed to install OpenCL via vcpkg.
	echo.
    exit /b 1
)

:: Success message
echo [SUCCESS] OpenCL has been successfully installed.
echo.
exit /b 0
