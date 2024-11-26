#!/bin/bash

# Step 1: Define paths
VCPKG_DIR="./vcpkg"
VCPKG_CMD="$VCPKG_DIR/vcpkg"
PACKAGE_NAME="opencl:x64-linux"

# Step 2: Check if vcpkg exists
if [[ ! -f "$VCPKG_CMD" ]]; then
    echo "vcpkg is not found in $VCPKG_DIR."
    echo "Please ensure vcpkg is correctly installed and try again."
    exit 1
fi

# Step 3: Check if OpenCL is installed
echo "Checking if OpenCL is installed..."
$VCPKG_CMD list | grep -i "$PACKAGE_NAME" > /dev/null
if [[ $? -eq 0 ]]; then
    echo "OpenCL is installed. Proceeding with uninstallation..."
else
    echo "OpenCL is not installed. Nothing to uninstall."
    exit 0
fi

# Step 4: Uninstall OpenCL
echo "Uninstalling OpenCL..."
$VCPKG_CMD remove "$PACKAGE_NAME"
if [[ $? -ne 0 ]]; then
    echo "Failed to uninstall OpenCL via vcpkg."
    exit 1
fi

# Step 5: Success message
echo "OpenCL has been successfully uninstalled."
exit 0
