# Sensor Stack Commands (AEye + Livox + Arena + Boson)

## Dependency List (Jetson, Ubuntu 22.04, ROS 2 Humble)

Assumption: this workspace is always run on Jetson with ROS 2 Humble already installed under `/opt/ros/humble`.

### System packages

```bash
sudo apt update
sudo apt install -y \
  build-essential \
  cmake \
  git \
  pkg-config \
  python3-dev \
  python3-pip \
  libeigen3-dev \
  libfmt-dev \
  libapr1-dev \
  libpcl-dev \
  libopencv-dev
```

### ROS 2 packages

```bash
sudo apt update
sudo apt install -y \
  ros-humble-rclcpp \
  ros-humble-rclcpp-components \
  ros-humble-std-msgs \
  ros-humble-std-srvs \
  ros-humble-sensor-msgs \
  ros-humble-geometry-msgs \
  ros-humble-rcl-interfaces \
  ros-humble-rcutils \
  ros-humble-tf2-ros \
  ros-humble-tf2-geometry-msgs \
  ros-humble-cv-bridge \
  ros-humble-pcl-conversions \
  ros-humble-ament-cmake \
  ros-humble-ament-cmake-auto \
  ros-humble-rosidl-default-generators \
  ros-humble-rosidl-default-runtime \
  ros-humble-ros2launch \
  ros-humble-rosbag2 \
  pybind11-dev
```

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

## Run MAVROS (global position + full quaternion orientation)

```bash
ros2 launch boson_camera mavros_global_position.launch.xml \
  fcu_url:=/dev/ttyTHS1:921600
```

### Optional Debug checks to ensure good data collection

#### Check global position rate

```bash
source /home/mg-nx-1/SARNIF_ws/sensor_stack_env.sh
timeout 10 ros2 topic hz /mavros/global_position/global
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
```bash
ros2 launch boson_camera sensor_stack.launch.py
```

`sensor_stack.launch.py` also starts MAVROS by default using `fcu_url:=/dev/ttyTHS1:921600` with a `global_position`-only plugin profile.