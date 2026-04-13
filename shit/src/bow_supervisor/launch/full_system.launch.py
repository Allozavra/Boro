from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    supervisor_params = os.path.join(
        get_package_share_directory('bow_supervisor'),
        'config',
        'supervisor_params.yaml'
    )
    controller_params = os.path.join(
        get_package_share_directory('inverse_dynamics_controller'),
        'config',
        'controller_params.yaml'
    )

    return LaunchDescription([
        DeclareLaunchArgument('use_sim_time', default_value='false'),

        Node(
            package='inverse_dynamics_controller',
            executable='inverse_dynamics_controller_node',
            name='inverse_dynamics_controller_node',
            output='screen',
            parameters=[controller_params, {'use_sim_time': LaunchConfiguration('use_sim_time')}],
        ),
        Node(
            package='bow_supervisor',
            executable='bow_supervisor_node',
            name='bow_supervisor_node',
            output='screen',
            parameters=[supervisor_params, {'use_sim_time': LaunchConfiguration('use_sim_time')}],
        ),
    ])
