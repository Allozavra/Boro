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
                'urdf_path': '/home/farall/Projects/Boro/ros2_ws/src/boro_description/urdf/boro.urdf',
                'contact_frame_name': 'bow_contact_frame',

                # Топики
                'input_topic': '/sim_ros',
                'output_topic': '/ros_sim',
                'target_topic': '/bow/target',
                'state_topic': '/bow/state',

                # Начальные цели
                'init_a_t': 0.0,
                'init_ad_t': 0.0,
                'init_add_t': 0.0,

                'init_h_t': 0.0,
                'init_hd_t': 0.0,
                'init_hdd_t': 0.0,

                'init_l_t': 0.0,
                'init_ld_t': 0.15,
                'init_ldd_t': 0.0,

                'init_force_t': 0.5,

                # Физика
                'Mu': 0.3,   # пока тангенциальную компенсацию лучше выключить

                # alpha вне контакта 
                # 'P_a_free': 20.0,
                # 'I_a_free': 0.0,
                # 'D_a_free': 9.0,
                'P_a_free': 50.0,
                'I_a_free': 4.0,
                'D_a_free': 16.0,

                # alpha в контакте
                'P_a_contact': 14.0,
                'I_a_contact': 0.0,
                'D_a_contact': 100.0,
                'K_tau_a_ff': 0.0,
                'alpha_integral_limit': 0.15,

                # h для режимов position / velocity
                'P_h': 64.0,
                'D_h': 16.0,
                'P_vh': 6.0,

                # l
                'P_l': 25.0,
                'D_l': 10.0,
                'P_vl': 100.0,

                # Импеданс по h в MODE_FORCE
                'M_h_imp': 0.6,
                'D_h_imp': 25.0,
                'K_h_imp': 12.0,
                'alpha_force': 0.0,

                'I_force_ref': 0.04,
                'h_force_ref_min': -0.08,
                'h_force_ref_max': 0.08,

                # Контакт
                'F_touch': 0.10,
                'F_lose': 0.05,

                # Ограничения
                'tau_a_max': 1000.0,
                'tau_h_max': 1000.0,
                'tau_l_max': 1000.0,

                'q1_dd_max': 1000.0,
                'q2_dd_max': 1000.0,
                'q3_dd_max': 1000.0,

                # Оси contact_frame
                'normal_axis_index': 2,
                'tangent_axis_index': 0,
            }]
        )
    ])