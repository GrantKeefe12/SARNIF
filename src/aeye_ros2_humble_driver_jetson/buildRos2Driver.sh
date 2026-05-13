#!/bin/bash

set -e

echo "Cleaning old build artifacts..."
sudo rm -rf ./install

echo "Building ROS2 Humble driver..."

if colcon build --packages-select aeye_msgs_humble aeye_ros2_driver_humble --install-base ./install --cmake-args -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON; then
    echo "Cleaning build artifacts..."
    rm -rf build log
    echo "Fresh build complete! New binaries are in: $(realpath ./install)"
else
    echo "Build failed!"
    exit 1
fi