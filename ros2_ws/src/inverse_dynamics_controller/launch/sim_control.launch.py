# colcon build --packages-select inverse_dynamics_controller
# source install/setup.bash
# ros2 launch inverse_dynamics_controller controller.launch.py


from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='inverse_dynamics_controller',
            executable='inverse_dynamics_controller_node',
            name='inverse_dynamics_controller_node',
            output='screen',
            parameters=[{
                # Конфигурация робота
                'urdf_path': '/home/farall/Projects/Boro/boro.urdf',
                'contact_frame_name': 'bow_contact_frame',

                # Топики
                'input_topic': '/sim_ros',
                'output_topic': '/ros_sim',

                # Цели
                'a_t': -0.26179938779914943653855361527329,         # Цель 1 звена (угол) (rad)
                'ad_t': 0.0,       # Цель 1 звена (угловая скорость)
                'add_t': 0.0,      # Цель 1 звена (угловое ускорение)

                'F_t': 0.4,             # Цель сила
                'Mu': 0.3,              # Коэффициент трения
                'hd_t': -0.1,
                'hdd_t': 0.0,
                'h_sign': -1.0,

                'ld_t': 0.05,          # Цель 3 звена (скорость)
                'ldd_t': 0.0,          # Цель 3 звена (ускорение)
                'l_min': -0.20,
                'l_max': 0.20,


                # ПИД коэффициенты
                'P_a': 280.0,
                'I_a': 10.0,     # NOT USED
                'D_a': 26.7,

                'P_F': 2.0,
                'I_F': 10.0,
                'D_F': 8.0,
                'D_hd': 30.0,

                'P_ld': 10.0,       # NOT USED
                'I_ld': 10.0,       # NOT USED
                'D_ld': 30.0,


                # Ограничения
                'tau_a_max': 1000.0,
                'tau_h_max': 10.0,
                'tau_l_max': 1000.0,

                'force_integral_limit': 10.0,

                # Локальные оси contact_frame:
                # 0 -> X, 1 -> Y, 2 -> Z
                'normal_axis_index': 2,
                'tangent_axis_index': 0,
            }]
        )
    ])