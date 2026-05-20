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
source /home/mg-nx-1/SARNIF_ws/sensor_stack_env.sh

ros2 launch boson_camera mavros_global_position.launch.xml \
  fcu_url:=/dev/ttyTHS1:921600
```

### Check global position rate

```bash
source /home/mg-nx-1/SARNIF_ws/sensor_stack_env.sh
timeout 10 ros2 topic hz /mavros/global_position/global
```

### Orientation topic (full quaternion)

```bash
source /home/mg-nx-1/SARNIF_ws/sensor_stack_env.sh
ros2 topic echo /mavros/imu/data --once
```

### Confirm minimal MAVROS topic set

```bash
source /home/mg-nx-1/SARNIF_ws/sensor_stack_env.sh
ros2 topic list | grep '^/mavros/'
```

Expected position + orientation signals:
- `/mavros/global_position/global`
- `/mavros/imu/data` (orientation quaternion x/y/z/w)

With the current plugin allowlist, MAVROS is reduced to only `global_position` and `imu` plugins to keep overhead low while providing full quaternion attitude.

**Measured Performance (with global_position + imu plugin config):**
- **Default out-of-box rate: ~35 Hz sustained** ✓ (excellent baseline, 2.3× improvement over previous config)
- Rate is stable and consistent (min/max inter-message intervals ~17-81 ms)
- Successfully logging to rosbag2 without drops

### Achieving Higher Rates (Target: 100 Hz)

**Current Ceiling Analysis:**
- Serial link (921600 baud) can support ~1,150 MAVLink messages/sec max
- GLOBAL_POSITION_INT message is ~20 bytes uncompressed
- **Serial bandwidth is NOT the bottleneck** (35 Hz = 700 bytes/sec, only 0.06% utilization)
- **Limit is ArduPilot firmware default stream rate** for this FCU

**To push beyond 35 Hz:**

1. **On ArduPilot FCU:** Increase stream rate parameter `SR1_POSITION` or `SR0_POSITION` (depending on FCU port assignment)
   - Via MAVSDK CLI or Mission Planner: set to 50+ Hz
   - Test if firmware supports 100+ Hz (some older versions cap at 50 Hz)

2. **Higher Baud Rate (optional, if rate ceiling permits):**
   - Increase FCU serial speed to 115200, 230400, 576000, or 921600+ (hardware-dependent)
   - Update `fcu_url` in mavros_global_position.launch.xml

3. **Test Rate Control via MAVROS** (if above doesn't work):
   - Temporarily add `command` plugin to plugin_allowlist
   - Call: `ros2 service call /mavros/set_stream_rate mavros_msgs/srv/StreamRate '{stream_id: 6, message_rate: 100, on_off: true}'`
   - Measure: `timeout 10 ros2 topic hz /mavros/global_position/global`

**Expected Reality:**
- 100 Hz on serial is unlikely unless FCU explicitly supports it (most older ArduPilot versions max out ~50 Hz default)
- 35 Hz baseline with global-position-only config is production-ready for most applications
- If 100 Hz required, recommend UDP network telemetry instead of serial

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

`sensor_stack.launch.py` now also starts MAVROS by default using `fcu_url:=/dev/ttyTHS1:921600` with a `global_position`-only plugin profile.

To override or disable it:

```bash
ros2 launch boson_camera sensor_stack.launch.py mavros_fcu_url:=/dev/ttyTHS1:921600
ros2 launch boson_camera sensor_stack.launch.py start_mavros:=false
```