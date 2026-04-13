cmake_minimum_required(VERSION 3.8)
project(boro_task_manager)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(boro_interfaces REQUIRED)

add_executable(boro_task_manager_node src/boro_task_manager_node.cpp)
ament_target_dependencies(boro_task_manager_node rclcpp sensor_msgs std_msgs boro_interfaces)

install(TARGETS boro_task_manager_node
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME})

ament_package()
