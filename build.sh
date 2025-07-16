#!/bin/bash

BUILD_DIR="build"

# create build directory if not exist
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR"

echo "Configuring the project with CMake..."
cmake ..

echo "Building the project..."
make

cd ..

echo "Build completed successfully!"
