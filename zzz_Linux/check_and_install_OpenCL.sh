#!/bin/bash

# Step 1: Define paths
VCPKG_DIR="./vcpkg"
VCPKG_CMD="$VCPKG_DIR/vcpkg"
PACKAGE_NAME="opencl:x64-linux"

# Step 2: Check if vcpkg exists
if [[ ! -f "$VCPKG_CMD" ]]; then
    echo "[ERROR] vcpkg is not found in $VCPKG_DIR."
    echo "[ERROR] Please ensure vcpkg is correctly installed and try again."
    echo
    exit 1
fi

# Step 3: Check if OpenCL is already installed
echo "[INFO] Checking if OpenCL is installed..."
echo
$VCPKG_CMD list | grep -i "$PACKAGE_NAME" > /dev/null
if [[ $? -eq 0 ]]; then
    echo "[INFO] OpenCL is already installed."
    echo
    exit 0
fi

# Step 4: Install OpenCL
echo "[INFO] OpenCL is not installed. Installing now..."
$VCPKG_CMD install "$PACKAGE_NAME"
if [[ $? -ne 0 ]]; then
    echo "[ERROR] Failed to install OpenCL via vcpkg."
    echo
    exit 1
fi

# Step 5: Success message
echo "[SUCCESS] OpenCL has been successfully installed."
echo
exit 0
