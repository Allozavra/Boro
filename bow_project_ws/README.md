# Bow project workspace

Новый ROS2-проект для модуля смычка:
- `bow_interfaces` — сообщения между state machine, supervisor и контроллером;
- `bow_supervisor` — нода верхнего уровня, формирует `BowTarget` и публикует события;
- `inverse_dynamics_controller` — обновленный низкоуровневый контроллер, принимает target вместо фиксированных параметров launch.

## Сборка
```bash
cd bow_project_ws
colcon build
source install/setup.bash
```

## Запуск
```bash
ros2 launch bow_supervisor full_system.launch.py
```

## Основные топики
- `/bow/command` (`bow_interfaces/msg/SupervisorCommand`)
- `/bow/target` (`bow_interfaces/msg/BowTarget`)
- `/bow/state` (`bow_interfaces/msg/BowState`)
- `/bow/event` (`bow_interfaces/msg/BowEvent`)
- `/sim_ros` (`sensor_msgs/msg/JointState`) — вход от Simulink/Simscape
- `/ros_sim` (`sensor_msgs/msg/JointState`) — усилия обратно в модель

## Пример команд
Выбор струны D:
```bash
ros2 topic pub --once /bow/command bow_interfaces/msg/SupervisorCommand "{command: 2, selected_string: 'D', auto_continue: true}"
```

Старт рабочего хода:
```bash
ros2 topic pub --once /bow/command bow_interfaces/msg/SupervisorCommand "{command: 4, desired_force: 0.4, bow_velocity: 0.05, bow_range_min: -0.16, bow_range_max: 0.16, auto_continue: true}"
```

Сброс ошибки:
```bash
ros2 topic pub --once /bow/command bow_interfaces/msg/SupervisorCommand "{command: 7}"
```
