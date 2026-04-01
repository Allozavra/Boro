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
                'init_a_t': -0.26179938779914943653855361527329,
                'init_ad_t': 0.0,
                'init_add_t': 0.0,

                'init_h_t': 0.0,
                'init_hd_t': 0.0,
                'init_hdd_t': 0.0,

                'init_l_t': 0.0,
                'init_ld_t': 0.0,
                'init_ldd_t': 0.0,
                
                # ПИД коэффициенты
                'Mu': 0.3,              # Коэффициент трения
                
                'P_a': 280.0,
                'D_a': 26.7,

                'P_h': 200.0,
                'D_h': 20.0,
                'P_l': 200.0,
                'D_l': 20.0,
                'P_vh': 10.0,
                'P_vl': 10.0,
                
                'P_F': 2.0,
                'I_F': 10.0,
                'D_F': 8.0,
                
                'F_touch': 0.10,
                'F_lose': 0.05,

                # Ограничения
                'force_integral_limit': 1000.0,
                'tau_a_max': 1000.0,
                'tau_h_max': 1000.0,
                'tau_l_max': 1000.0,
                'q1_dd_max': 1000.0,
                'q2_dd_max': 1000.0,
                'q3_dd_max': 1000.0,

                # Локальные оси contact_frame:
                # 0 -> X, 1 -> Y, 2 -> Z
                'normal_axis_index': 2,
                'tangent_axis_index': 0,
            }]
        )
    ])