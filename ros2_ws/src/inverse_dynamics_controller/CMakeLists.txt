cmake_minimum_required(VERSION 3.8)
project(inverse_dynamics_controller)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(pinocchio REQUIRED)
find_package(boro_interfaces REQUIRED)

add_executable(inverse_dynamics_controller_node
  src/inverse_dynamics_controller_node.cpp
)

target_include_directories(inverse_dynamics_controller_node PRIVATE
  ${EIGEN3_INCLUDE_DIRS}
)

ament_target_dependencies(inverse_dynamics_controller_node
  rclcpp
  sensor_msgs
  boro_interfaces
)

target_link_libraries(inverse_dynamics_controller_node
  pinocchio::pinocchio
)

install(TARGETS
  inverse_dynamics_controller_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY config launch config
  DESTINATION share/${PROJECT_NAME}
)

ament_package()