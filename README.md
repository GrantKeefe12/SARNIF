# Sensor Stack Commands (AEye + Livox + Arena + Boson)

## Dependency List (Jetson, Ubuntu 22.04, ROS 2 Humble, Cyclone DDS)

Assumption: this workspace is always run on Jetson with ROS 2 Humble already installed under `/opt/ros/humble`.


### SDK / vendor prerequisites

1. Arena SDK (required by `arena_camera_node`): install LUCID ArenaSDK and ensure `/etc/ld.so.conf.d/Arena_SDK.conf` exists.
2. AEye SDK (required by `aeye_ros2_driver_humble`): provide SDK at `src/aeye_ros2_humble_driver_jetson/sdk_build` with:
   - `lib/libAEyeSensorSDK.a`
   - `lib/cmake/libAEyeSensorSDK/*`
   - `include/*`


## Build

```bash
cd /home/mg-nx-1/SARNIF_ws
source /opt/ros/humble/setup.bash
colcon build --symlink-install
```

## Connecting to the Plane


```bash
ssh mg-nx-1@10.223.0.9

password: mg-nx-1
```

## Connecting to the M600

Use the BEC router, Network SSID: SARNIF, password: 4EB3A044

If this isn't working give the plane a few minutes to boot and configure etherntet

## Source (every terminal)

ALERT:

```bash
source /home/mg-nx-1/SARNIF_ws/sensor_stack_env.sh
```
M600:
```bash
source /home/mg-nx-2/SARNIF/sensor_stack_env.sh
```

## Run AEye

```bash

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

## Run MAVROS (global position + full quaternion orientation)

```bash
ros2 launch boson_camera mavros_global_position.launch.xml \
  fcu_url:=/dev/ttyTHS1:921600
```

## Launch All
```bash
ros2 launch boson_camera sensor_stack.launch.py
```

`sensor_stack.launch.py` also starts MAVROS by default using `fcu_url:=/dev/ttyTHS1:921600` with a `global_position`-only plugin profile.

## Data recording

```bash
ros2 bag record \
  /rgb_image \
  /livox/lidar \
  /mavros/global_position/global \
  /mavros/imu/data \
  -o bags/<bag name>
```