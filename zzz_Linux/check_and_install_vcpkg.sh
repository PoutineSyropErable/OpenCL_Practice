#!/bin/bash

# Step 1: Define vcpkg directory and command
VCPKG_DIR="./vcpkg"
VCPKG_CMD="$VCPKG_DIR/vcpkg"

# Step 2: Check if vcpkg directory exists
if [[ ! -d "$VCPKG_DIR" ]]; then
    echo "[INFO] vcpkg directory not found. Cloning vcpkg..."
    echo
    git clone https://github.com/microsoft/vcpkg.git "$VCPKG_DIR" || {
        echo "[ERROR] Failed to clone vcpkg. Ensure Git is installed and accessible."
        exit 1
    }
else
    echo "[INFO] vcpkg directory already exists."
    echo
fi

# Step 3: Check if vcpkg executable exists
if [[ ! -f "$VCPKG_CMD" ]]; then
    echo "[INFO] vcpkg executable not found. Bootstrapping vcpkg..."
    echo
    pushd "$VCPKG_DIR" > /dev/null
    ./bootstrap-vcpkg.sh || {
        echo "[ERROR] Failed to bootstrap vcpkg. Ensure prerequisites are installed."
        echo
        popd > /dev/null
        exit 1
    }
    popd > /dev/null
else
    echo "[INFO] vcpkg executable found."
    echo
fi

# Step 4: Confirm vcpkg is ready
if [[ ! -f "$VCPKG_CMD" ]]; then
    echo "[ERROR] vcpkg setup failed. Please check the logs and try again."
    echo
    exit 1
fi

# Step 5: Success message
echo "[INFO] vcpkg is ready."
echo
echo "[SUCCESS] vcpkg got correctly installed"
