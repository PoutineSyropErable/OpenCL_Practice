#!/bin/bash

# Step 1: Call the script to check and install vcpkg and OpenCL
echo "[INFO] Checking and Installing vcpkg and OpenCL..."
echo
./check_and_install_both.sh
if [[ $? -ne 0 ]]; then
    echo "[ERROR] Failed to set up vcpkg and OpenCL. Exiting..."
    echo
    exit 1
fi
echo "[SUCCESS] vcpkg and OpenCL are successfully set up."
echo

# Step 2: Define paths
BUILD_DIR="build"
EXECUTABLE="VectorAdditionOnGraphicsCard"

# Step 3: Check if the build directory exists; if not, create it
if [[ ! -d "$BUILD_DIR" ]]; then
    echo "[INFO] Creating build directory: $BUILD_DIR"
    echo
    mkdir "$BUILD_DIR"
else
    echo "[INFO] Build directory already exists: $BUILD_DIR"
    echo
fi

# Step 4: Run CMake to configure the project
echo "[INFO] Creating the Makefile using the CMake configuration file."
echo
cmake -S . -B "$BUILD_DIR"
if [[ $? -ne 0 ]]; then
    echo
    echo "[ERROR] CMake configuration failed!"
    echo
    exit 1
fi

# Step 5: Build the project
echo "[INFO] Building the Project..."
echo
cmake --build "$BUILD_DIR"
if [[ $? -ne 0 ]]; then
    echo
    echo "[ERROR] Build failed!"
    echo
    exit 1
fi
echo "[SUCCESS] Project Build Successful."
echo

# Step 6: Check if the executable exists
if [[ ! -f "$BUILD_DIR/$EXECUTABLE" ]]; then
    echo
    echo "[ERROR] Build succeeded but executable not found in $BUILD_DIR!"
    echo "[ERROR] Please check your CMake configuration and paths."
    echo
    exit 1
fi
echo "[INFO] Executable found: $BUILD_DIR/$EXECUTABLE"
echo

# Step 7: Run the executable
echo "[INFO] Running the program..."
echo
"./$BUILD_DIR/$EXECUTABLE"
if [[ $? -ne 0 ]]; then
    echo
    echo "[ERROR] Program execution failed!"
    echo
    exit 1
fi

# Step 8: Success message
echo
echo "[SUCCESS] Program executed successfully!"
echo
