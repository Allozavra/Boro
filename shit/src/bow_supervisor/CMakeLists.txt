cmake_minimum_required(VERSION 3.8)
project(bow_supervisor)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(bow_interfaces REQUIRED)

add_executable(bow_supervisor_node src/bow_supervisor_node.cpp)
ament_target_dependencies(bow_supervisor_node rclcpp sensor_msgs std_msgs bow_interfaces)

install(TARGETS bow_supervisor_node
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME})

ament_package()
