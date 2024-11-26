#!/bin/bash

# Step 1: Check and install dependencies
echo "[INFO] Checking and installing dependencies..."
echo
./check_and_install_both.sh
if [[ $? -ne 0 ]]; then
    echo "[ERROR] Failed to set up vcpkg and OpenCL. Exiting..."
    echo
    exit 1
fi
echo "[SUCCESS] Dependencies are set up."
echo

# Step 2: Define paths and variables
INCLUDE_PATH="./vcpkg/installed/x64-linux/include"
LIB_PATH="./vcpkg/installed/x64-linux/lib"
BUILD_DIR="linux_build"
OUTPUT="$BUILD_DIR/VectorAdditionOnGraphicsCard"

# Step 3: Create build directory if it doesn't exist
if [[ ! -d "$BUILD_DIR" ]]; then
    echo "[INFO] Creating build directory: $BUILD_DIR"
    echo
    mkdir -p "$BUILD_DIR"
else
    echo "[INFO] Build directory already exists: $BUILD_DIR"
    echo
fi
echo

# Step 4: Clean previous build
echo "[INFO] Cleaning previous build..."
echo
rm -f "$OUTPUT"
rm -f "$BUILD_DIR"/*.o
echo "[SUCCESS] Previous build cleaned."
echo

# Step 5: Compile and link
echo "[INFO] Compiling and linking the project..."
echo
gcc -I"$INCLUDE_PATH" main.c -o "$OUTPUT" -L"$LIB_PATH" -lOpenCL
if [[ $? -ne 0 ]]; then
    echo "[ERROR] Build failed during compilation and linking!"
    echo
    exit 1
fi
echo "[SUCCESS] Build succeeded! Executable: $OUTPUT"
echo

# Step 6: Check for executable
if [[ ! -f "$OUTPUT" ]]; then
    echo "[ERROR] Build succeeded but executable not found in $BUILD_DIR!"
    echo
    exit 1
fi
echo "[INFO] Executable found: $OUTPUT"
echo

# Step 7: Run the program
echo "[INFO] Running the program..."
echo
"$OUTPUT"
if [[ $? -ne 0 ]]; then
    echo "[ERROR] Program execution failed!"
    echo
    exit 1
fi
echo "[SUCCESS] Program executed successfully!"
echo
