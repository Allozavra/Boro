from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    params = os.path.join(
        get_package_share_directory('inverse_dynamics_controller'),
        'config',
        'controller_params.yaml'
    )

    return LaunchDescription([
        Node(
            package='inverse_dynamics_controller',
            executable='inverse_dynamics_controller_node',
            name='inverse_dynamics_controller_node',
            output='screen',
            parameters=[params],
        )
    ])
