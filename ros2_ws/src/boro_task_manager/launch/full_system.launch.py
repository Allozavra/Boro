from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    task_manager_config = PathJoinSubstitution([
        FindPackageShare("boro_task_manager"),
        "config",
        "task_params.yaml",
    ])

    controller_config = PathJoinSubstitution([
        FindPackageShare("inverse_dynamics_controller"),
        "config",
        "controller_params.yaml",
    ])

    urdf_path = PathJoinSubstitution([
        FindPackageShare("boro_description"),
        "urdf",
        "boro.urdf",
    ])

    return LaunchDescription([
        Node(
            package="boro_task_manager",
            executable="boro_task_manager_node",
            name="boro_task_manager_node",
            output="screen",
            parameters=[task_manager_config],
        ),
        Node(
            package="inverse_dynamics_controller",
            executable="inverse_dynamics_controller_node",
            name="inverse_dynamics_controller_node",
            output="screen",
            parameters=[
                controller_config,
                {
                    "urdf_path": urdf_path,
                },
            ],
        ),
    ])