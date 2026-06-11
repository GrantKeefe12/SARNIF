#!/bin/bash

source /opt/ros/humble/setup.bash
source /home/mg-nx-2/SARNIF/install/setup.bash

export ROS_DOMAIN_ID=42
export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp
export CYCLONEDDS_URI=/home/mg-nx-2/SARNIF/install/aeye_ros2_driver_humble/share/aeye_ros2_driver_humble/rviz/cyclonedds.xml

echo "ROS_DOMAIN_ID=$ROS_DOMAIN_ID"
echo "RMW_IMPLEMENTATION=$RMW_IMPLEMENTATION"
echo "CYCLONEDDS_URI=$CYCLONEDDS_URI"
