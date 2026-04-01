cmake_minimum_required(VERSION 3.8)
project(bow_interfaces)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(std_msgs REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/BowTarget.msg"
  "msg/BowState.msg"
  "msg/BowEvent.msg"
  "msg/SupervisorCommand.msg"
  DEPENDENCIES std_msgs
)

ament_export_dependencies(rosidl_default_runtime)
ament_package()
