@echo off

echo [INFO] Checking and Installing Vcpkg and OpenCL
echo.

echo [INFO] Checking and Installing Vcpkg
echo.
:: Call check_and_install_vcpkg.bat
call check_and_install_vcpkg.bat
if %errorlevel% neq 0 (
    echo [ERROR] Failed to set up vcpkg. Exiting...
    exit /b 1
)

echo [INFO] Checking and Installing OpenCL
echo.
:: Call check_and_install_openCL.bat
call check_and_install_openCL.bat
if %errorlevel% neq 0 (
    echo [ERROR] Failed to set up OpenCL. Exiting...
	echo.
    exit /b 1
)

:: Success message
echo [SUCCESS] vcpkg and OpenCL are successfully installed.
echo.
exit /b 0
