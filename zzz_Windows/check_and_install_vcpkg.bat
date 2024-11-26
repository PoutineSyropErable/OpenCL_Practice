@echo off

:: Define vcpkg directory and command
set VCPKG_DIR=.\vcpkg
set VCPKG_CMD=%VCPKG_DIR%\vcpkg.exe

:: Check if vcpkg directory exists
if not exist %VCPKG_DIR% (
    echo [INFO] vcpkg directory not found. Cloning vcpkg...
	echo.
    git clone https://github.com/microsoft/vcpkg.git %VCPKG_DIR% || (
        echo [ERROR] Failed to clone vcpkg. Ensure Git is installed and accessible.
        exit /b 1
    )
) else (
    echo [INFO] vcpkg directory already exists.
)

echo.

:: Check if vcpkg.exe exists
if not exist %VCPKG_CMD% (
    echo [INFO] vcpkg executable not found. Bootstrapping vcpkg...
	echo.
    pushd %VCPKG_DIR%
    .\bootstrap-vcpkg.bat || (
        echo [ERROR] Failed to bootstrap vcpkg. Ensure prerequisites are installed.
		echo.
        popd
        exit /b 1
    )
    popd
) else (
    echo [INFO] vcpkg executable found.
	echo.
)

echo.

:: Confirm vcpkg is ready
if not exist %VCPKG_CMD% (
    echo [ERROR] vcpkg setup failed. Please check the logs and try again.
	echo.
    exit /b 1
)

echo [INFO] vcpkg is ready.
echo.



:: Success message
echo [SUCCESS] vcpkg successfully set up.
echo.
exit /b 0


