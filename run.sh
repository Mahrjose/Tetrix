#!/usr/bin/env bash

# Check if build directory exists
if [ ! -d "build" ]; then
    mkdir -p build && cd build || exit 1
    cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug .. || {
        echo "CMake configuration failed!"
        exit 1
    }
else
    cd build || exit 1
fi

# Compile the project
ninja || {
    echo "Build failed!"
    exit 1
}

# Run the application
if [ -f "./bin/Tetrix" ]; then
    ./bin/Tetrix
else
    echo "Build failed! Executable not found."
fi
