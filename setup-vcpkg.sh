#!/bin/bash

# Exit immediately if any command fails
set -e

# Save the current working directory
ORIGINAL_DIR=$(pwd)

# Define the target directory for vcpkg
VCPKG_DIR="./thirdparty/vcpkg"

# Function to ensure returning to the original directory
cleanup() {
    cd "$ORIGINAL_DIR"
}
trap cleanup EXIT

# Check if the thirdparty directory exists, create it if it doesn't
if [ ! -d "./thirdparty" ]; then
    echo "Creating thirdparty directory..."
    mkdir -p "./thirdparty"
fi

# Clone vcpkg if it doesn't already exist in the thirdparty directory
if [ ! -d "$VCPKG_DIR" ]; then
    echo "Cloning vcpkg into $VCPKG_DIR..."
    git clone https://github.com/microsoft/vcpkg.git "$VCPKG_DIR"
else
    echo "vcpkg is already cloned in $VCPKG_DIR"
fi

# Change to the vcpkg directory
cd "$VCPKG_DIR"

# Make sure the bootstrap script is executable
echo "Making the bootstrap script executable..."
chmod +x ./bootstrap-vcpkg.sh

# Run the bootstrap script
echo "Running the bootstrap script..."
./bootstrap-vcpkg.sh

# Script completed successfully
echo "vcpkg has been successfully bootstrapped!"
