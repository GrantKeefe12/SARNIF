import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.launch_description_sources import AnyLaunchDescriptionSource
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.actions import IncludeLaunchDescription
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    start_mavros_arg = DeclareLaunchArgument(
        'start_mavros',
        default_value='true',
    )
    mavros_fcu_url_arg = DeclareLaunchArgument(
        'mavros_fcu_url',
        default_value='/dev/ttyTHS1:921600',
    )
    mavros_namespace_arg = DeclareLaunchArgument(
        'mavros_namespace',
        default_value='mavros',
    )

    livox_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(
                get_package_share_directory('livox_ros2_avia'),
                'launch',
                'livox_lidar_launch.py',
            )
        )
    )

    mavros_launch = IncludeLaunchDescription(
        AnyLaunchDescriptionSource(
            os.path.join(
                get_package_share_directory('boson_camera'),
                'launch',
                'mavros_global_position.launch.xml',
            )
        ),
        launch_arguments={
            'fcu_url': LaunchConfiguration('mavros_fcu_url'),
            'namespace': LaunchConfiguration('mavros_namespace'),
        }.items(),
        condition=IfCondition(LaunchConfiguration('start_mavros')),
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
        start_mavros_arg,
        mavros_fcu_url_arg,
        mavros_namespace_arg,
        mavros_launch,
        livox_launch,
        arena_camera,
        bayer_to_rgb,
    ])
