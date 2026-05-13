# ROS2 Humble Driver

Standalone AEye ROS2 driver for Humble with unique package names.
This folder vendors the Jetson-compatible AArch64 AEye SDK under `./sdk_build`.

## Build

```bash
cd /home/mg-nx-1/SARNIF_ws/src/aeye_ros2_humble_driver_jetson
source /opt/ros/humble/setup.bash
chmod +x buildRos2Driver.sh
./buildRos2Driver.sh
```

The build uses the local ARM64 SDK shipped in this folder, so it does not depend
on the original Apollo ROS driver package.

## Run

Before starting the driver on a real sensor, prepare the sensor-facing NIC.
For your current setup this is `enP8p1s0`.

```bash
cd /home/mg-nx-1/SARNIF_ws/src/aeye_ros2_humble_driver_jetson
chmod +x configure_aeye_network.sh
sudo ./configure_aeye_network.sh enP8p1s0
```

This raises the UDP receive buffers to `52428800` and sets the NIC MTU to `9000`.

```bash
cd /home/mg-nx-1/SARNIF_ws/src/aeye_ros2_humble_driver_jetson
source /opt/ros/humble/setup.bash
source ./install/setup.bash
ros2 launch aeye_ros2_driver_humble aeye_ros2_driver_humble_launch.py sensor_ip:=10.10.10.32 sensor_name:=apl_#32
```

If automatic network configuration is left enabled in the launch file, start it with sudo.
For your sensor, use `sensor_ip:=10.10.10.178`.