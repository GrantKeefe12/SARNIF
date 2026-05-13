# Sensor Stack Commands (AEye + Livox + Arena + Boson)

## Build

```bash
cd /home/mg-nx-1/SARNIF_ws
source /opt/ros/humble/setup.bash
colcon build --symlink-install
```

## Source (every terminal)

```bash
source /home/mg-nx-1/SARNIF_ws/sensor_stack_env.sh
```

## Run AEye

```bash
pkill -f 'aeye_ros2_driver_humble_launch.py|aeye_ros2_driver_humble_node' || true
source /home/mg-nx-1/SARNIF_ws/sensor_stack_env.sh

ros2 launch aeye_ros2_driver_humble aeye_ros2_driver_humble_launch.py \
  sensor_ip:=10.10.10.178 \
  sensor_name:=Apollo_178 \
  auto_network_configuration:=false \
  start_rviz:=false
```

## Run Livox

```bash
ros2 launch livox_ros2_avia livox_lidar_launch.py
```

## Run Arena (Triton)

```bash
pkill -f '/arena_camera_node/start|bayer_to_rgb_node|rviz2|ros2 topic hz /arena_camera_node/images|ros2 topic hz /rgb_image' || true

ros2 run arena_camera_node start --ros-args   -p pixelformat:=bayer_rggb8   -p topic:=/arena_camera_node/images
```

## Convert Arena Bayer to RGB

```bash
ros2 run boson_camera bayer_to_rgb_node --ros-args -p bayer_pattern:=bggr -p output_encoding:=rgb8
```

## Run Boson Thermal

```bash
ros2 run boson_camera boson_camera_node y
```

## Launch All

ros2 launch boson_camera sensor_stack.launch.py