import os
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    return_1_enabled = True
    return_2_enabled = True

    sensor_ip_arg = DeclareLaunchArgument(
        'sensor_ip',
        default_value='10.10.10.108',
        description='IP address of the LiDAR sensor'
    )
    sensor_name_arg = DeclareLaunchArgument(
        'sensor_name',
        default_value='Apollo_#5',
        description='Name of the LiDAR sensor'
    )
    auto_network_configuration_arg = DeclareLaunchArgument(
        'auto_network_configuration',
        default_value='false',
        description='Enable automatic network configuration (requires administrator privileges)'
    )
    start_rviz_arg = DeclareLaunchArgument(
        'start_rviz',
        default_value='false',
        description='Start RViz alongside the driver'
    )

    driver_node = Node(
        package='aeye_ros2_driver_humble',
        executable='aeye_ros2_driver_humble_node',
        name='aeye_ros2_driver_humble_node',
        output='screen',
        parameters=[{
            'sensor_ip': LaunchConfiguration('sensor_ip'),
            'sensor_name': LaunchConfiguration('sensor_name'),
            'auto_network_configuration': LaunchConfiguration('auto_network_configuration'),
            'return_1': return_1_enabled,
            'return_2': return_2_enabled
        }]
    )

    rviz_config_path = os.path.join(
        get_package_share_directory('aeye_ros2_driver_humble'),
        'rviz',
        'default.rviz'
    )

    rviz_process = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
        arguments=['-d', rviz_config_path],
        condition=IfCondition(LaunchConfiguration('start_rviz'))
    )

    return LaunchDescription([
        sensor_ip_arg,
        sensor_name_arg,
        auto_network_configuration_arg,
        start_rviz_arg,
        driver_node,
        rviz_process,
    ])