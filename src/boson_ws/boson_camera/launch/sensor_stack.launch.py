import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.actions import IncludeLaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    livox_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(
                get_package_share_directory('livox_ros2_avia'),
                'launch',
                'livox_lidar_launch.py',
            )
        )
    )

    aeye_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(
                get_package_share_directory('aeye_ros2_driver_humble'),
                'launch',
                'aeye_ros2_driver_humble_launch.py',
            )
        ),
        launch_arguments={
            'sensor_ip': '10.10.10.178',
            'sensor_name': 'Apollo_178',
            'auto_network_configuration': 'false',
            'start_rviz': 'false',
        }.items(),
    )

    arena_camera = Node(
        package='arena_camera_node',
        executable='start',
        name='arena_camera_node',
        output='screen',
        arguments=[
            '--ros-args',
            '-p', 'pixelformat:=bayer_rggb8',
            '-p', 'topic:=/arena_camera_node/images',
        ],
    )

    bayer_to_rgb = Node(
        package='boson_camera',
        executable='bayer_to_rgb_node',
        name='bayer_to_rgb_node',
        output='screen',
        arguments=['--ros-args', '-p', 'bayer_pattern:=bggr', '-p', 'output_encoding:=rgb8'],
    )

    boson_camera = Node(
        package='boson_camera',
        executable='boson_camera_node',
        name='boson_camera_node',
        output='screen',
        arguments=['y'],
    )

    livox_static_tf = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        name='livox_static_transform_publisher',
        output='screen',
        arguments=['0', '0', '0', '0', '0', '0', 'base_link', 'livox_frame'],
    )

    return LaunchDescription([
        livox_launch,
        aeye_launch,
        arena_camera,
        bayer_to_rgb,
        boson_camera,
        livox_static_tf,
    ])
