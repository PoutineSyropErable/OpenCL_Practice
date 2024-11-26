#!/bin/bash

# Step 1: Checking and Installing Vcpkg and OpenCL
echo "[INFO] Checking and Installing Vcpkg and OpenCL"
echo

# Step 2: Checking and Installing Vcpkg
echo "[INFO] Checking and Installing Vcpkg"
echo
./check_and_install_vcpkg.sh
if [[ $? -ne 0 ]]; then
    echo "[ERROR] Failed to set up vcpkg. Exiting..."
    echo
    exit 1
fi

# Step 3: Checking and Installing OpenCL
echo "[INFO] Checking and Installing OpenCL"
echo
./check_and_install_OpenCL.sh
if [[ $? -ne 0 ]]; then
    echo "[ERROR] Failed to set up OpenCL. Exiting..."
    echo
    exit 1
fi

# Step 4: Success Message
echo "[SUCCESS] vcpkg and OpenCL are successfully installed."
echo
exit 0
