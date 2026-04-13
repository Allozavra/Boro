cmake_minimum_required(VERSION 3.8)
project(inverse_dynamics_controller)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(bow_interfaces REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(pinocchio REQUIRED)

add_executable(inverse_dynamics_controller_node src/inverse_dynamics_controller_node.cpp)
ament_target_dependencies(inverse_dynamics_controller_node rclcpp sensor_msgs std_msgs bow_interfaces)
target_link_libraries(inverse_dynamics_controller_node Eigen3::Eigen pinocchio::pinocchio)

target_compile_features(inverse_dynamics_controller_node PUBLIC cxx_std_17)

install(TARGETS inverse_dynamics_controller_node
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME})

ament_package()
