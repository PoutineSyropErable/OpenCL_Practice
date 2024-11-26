@echo off
:: Set paths to OpenCL headers and libraries (quote paths to handle spaces)
set INCLUDE_PATH="E:\School and Work\University (real)\Semester 9\Personal\Run code GPU\vcpkg\installed\x64-windows\include"
set LIB_PATH="E:\School and Work\University (real)\Semester 9\Personal\Run code GPU\vcpkg\installed\x64-windows\lib"

:: Set output executable name
set OUTPUT=HelloOpenCL.exe

:: Clean previous build
if exist %OUTPUT% del %OUTPUT%

:: Compile and link (quote paths to avoid issues)
cl /nologo /I%INCLUDE_PATH% main.c /Fe:%OUTPUT% /link /LIBPATH:%LIB_PATH% OpenCL.lib

:: Check if the build succeeded
if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

:: Success message
echo Build succeeded! Executable: %OUTPUT%

:: Run the program
echo Running the program...
%OUTPUT%
if errorlevel 1 (
    echo Program execution failed.
    exit /b 1
)

:: Success message
echo Program executed successfully!

