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

    return LaunchDescription([
        livox_launch,
        arena_camera,
        bayer_to_rgb,
    ])
