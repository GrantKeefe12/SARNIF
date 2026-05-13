#!/bin/bash

# Shared ROS2 environment for running AEye + Livox + Arena + Boson together.
source /opt/ros/humble/setup.bash
source /home/mg-nx-1/SARNIF_ws/install/setup.bash

export ROS_DOMAIN_ID=42
export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp
export CYCLONEDDS_URI=/home/mg-nx-1/SARNIF_ws/src/aeye_ros2_humble_driver_jetson/src/aeye_ros2_driver_humble/rviz/cyclonedds.xml

echo "ROS_DOMAIN_ID=${ROS_DOMAIN_ID}"
echo "RMW_IMPLEMENTATION=${RMW_IMPLEMENTATION}"
echo "CYCLONEDDS_URI=${CYCLONEDDS_URI}"

# ArenaSDK is required by arena_camera_node.
if [[ -f /etc/ld.so.conf.d/Arena_SDK.conf ]]; then
	echo "ArenaSDK config: /etc/ld.so.conf.d/Arena_SDK.conf"
else
	echo "WARNING: ArenaSDK config not found at /etc/ld.so.conf.d/Arena_SDK.conf"
fi

# Boson camera defaults to /dev/video0 in boson_camera_node.
if [[ -e /dev/video0 ]]; then
	echo "Boson V4L2 device visible: /dev/video0"
else
	echo "WARNING: /dev/video0 not found (check Boson connection/device index)."
fi

