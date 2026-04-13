source install/setup.bash

colcon build --cmake-args -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

ros2 launch inverse_dynamics_controller controller_launch.py
ros2 launch boro_task_manager full_system.launch.py

ros2 topic echo /bow/command
ros2 topic echo /bow/event
ros2 topic echo /bow/target
ros2 topic echo /sim_ros
ros2 topic echo /ros_sim

ros2 launch foxglove_bridge foxglove_bridge_launch.xml