@echo off

:: Define paths
set VCPKG_DIR=.\vcpkg
set VCPKG_CMD=%VCPKG_DIR%\vcpkg.exe

:: Define OpenCL package name
set PACKAGE_NAME=opencl:x64-windows

:: Check if vcpkg exists
if not exist %VCPKG_CMD% (
    echo vcpkg is not found in %VCPKG_DIR%.
    echo Please ensure vcpkg is correctly installed and try again.
    exit /b 1
)

:: Check if OpenCL is installed
echo Checking if OpenCL is installed...
%VCPKG_CMD% list | findstr /i /c:"%PACKAGE_NAME%" >nul
if %errorlevel%==0 (
    echo OpenCL is installed. Proceeding with uninstallation...
) else (
    echo OpenCL is not installed. Nothing to uninstall.
    exit /b 0
)

:: Uninstall OpenCL
echo Uninstalling OpenCL...
%VCPKG_CMD% remove %PACKAGE_NAME%
if %errorlevel% neq 0 (
    echo Failed to uninstall OpenCL via vcpkg.
    exit /b 1
)

:: Success message
echo OpenCL has been successfully uninstalled.
exit /b 0
