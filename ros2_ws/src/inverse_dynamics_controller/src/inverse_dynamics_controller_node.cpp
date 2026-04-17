#include <Eigen/Dense>
#include <cmath>
#include <memory>
#include <pinocchio/algorithm/crba.hpp>
#include <pinocchio/algorithm/frames.hpp>
#include <pinocchio/algorithm/jacobian.hpp>
#include <pinocchio/algorithm/kinematics.hpp>
#include <pinocchio/algorithm/rnea.hpp>
#include <pinocchio/fwd.hpp>
#include <pinocchio/parsers/urdf.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <stdexcept>
#include <string>
#include <vector>

#include "boro_interfaces/msg/bow_state.hpp"
#include "boro_interfaces/msg/bow_target.hpp"

class InverseDynamicsControllerNode : public rclcpp::Node {
 public:
  InverseDynamicsControllerNode()
      : Node("inverse_dynamics_controller_node"),
        alpha_integral_(0.0),
        l_velocity_integral_(0.0),
        force_integral_(0.0),
        target_received_(false),
        in_contact_(false) {
    urdf_path_ = declare_parameter<std::string>("urdf_path", "");
    contact_frame_name_ = declare_parameter<std::string>("contact_frame_name", "bow_contact_frame");

    input_topic_ = declare_parameter<std::string>("input_topic", "/sim_ros");
    output_topic_ = declare_parameter<std::string>("output_topic", "/ros_sim");
    target_topic_ = declare_parameter<std::string>("target_topic", "/bow/target");
    state_topic_ = declare_parameter<std::string>("state_topic", "/bow/state");

    init_a_t_ = declare_parameter<double>("init_a_t", 0.0);
    init_ad_t_ = declare_parameter<double>("init_ad_t", 0.0);
    init_add_t_ = declare_parameter<double>("init_add_t", 0.0);
    init_h_t_ = declare_parameter<double>("init_h_t", 0.0);
    init_hd_t_ = declare_parameter<double>("init_hd_t", 0.0);
    init_hdd_t_ = declare_parameter<double>("init_hdd_t", 0.0);
    init_l_t_ = declare_parameter<double>("init_l_t", 0.0);
    init_ld_t_ = declare_parameter<double>("init_ld_t", 0.0);
    init_ldd_t_ = declare_parameter<double>("init_ldd_t", 0.0);
    init_force_t_ = declare_parameter<double>("init_force_t", 0.0);

    Mu_ = declare_parameter<double>("Mu", 0.3);

    // PID для alpha без контакта
    P_a_free_ = declare_parameter<double>("P_a_free", 0.0);
    I_a_free_ = declare_parameter<double>("I_a_free", 0.0);
    D_a_free_ = declare_parameter<double>("D_a_free", 0.0);

    // PID для alpha при контакте
    P_a_contact_ = declare_parameter<double>("P_a_contact", P_a_free_);
    I_a_contact_ = declare_parameter<double>("I_a_contact", I_a_free_);
    D_a_contact_ = declare_parameter<double>("D_a_contact", D_a_free_);

    P_h_ = declare_parameter<double>("P_h", 0.0);
    D_h_ = declare_parameter<double>("D_h", 0.0);
    P_l_ = declare_parameter<double>("P_l", 0.0);
    D_l_ = declare_parameter<double>("D_l", 0.0);
    P_vh_ = declare_parameter<double>("P_vh", 10.0);
    P_vl_ = declare_parameter<double>("P_vl", 10.0);
    I_vl_ = declare_parameter<double>("I_vl", 10.0);
    P_F_ = declare_parameter<double>("P_F", 0.0);
    I_F_ = declare_parameter<double>("I_F", 0.0);
    D_F_ = declare_parameter<double>("D_F", 0.0);

    F_touch_ = declare_parameter<double>("F_touch", 0.10);
    F_lose_ = declare_parameter<double>("F_lose", 0.05);
    F_integral_limit_ = declare_parameter<double>("force_integral_limit", 10.0);
    alpha_integral_limit_ = declare_parameter<double>("alpha_integral_limit", 10.0);
    l_velocity_integral_limit_ = declare_parameter<double>("l_velocity_integral_limit", 10.0);

    tau_a_max_ = declare_parameter<double>("tau_a_max", 1000.0);
    tau_h_max_ = declare_parameter<double>("tau_h_max", 1000.0);
    tau_l_max_ = declare_parameter<double>("tau_l_max", 1000.0);

    q1_dd_max_ = declare_parameter<double>("q1_dd_max", 1000.0);
    q2_dd_max_ = declare_parameter<double>("q2_dd_max", 1000.0);
    q3_dd_max_ = declare_parameter<double>("q3_dd_max", 1000.0);

    normal_axis_index_ = declare_parameter<int>("normal_axis_index", 2);
    tangent_axis_index_ = declare_parameter<int>("tangent_axis_index", 0);

    alpha_ctrl_ = declare_parameter<double>("alpha_ctrl", 0.0);
    K_contact_ff_ = declare_parameter<double>("K_contact_ff", 1.0);

    if (normal_axis_index_ < 0 || normal_axis_index_ > 2 || tangent_axis_index_ < 0 ||
        tangent_axis_index_ > 2 || normal_axis_index_ == tangent_axis_index_) {
      throw std::runtime_error(
          "normal_axis_index and tangent_axis_index must differ and be in [0,2]");
    }

    initPinocchio(urdf_path_, contact_frame_name_);

    sub_sim_ = create_subscription<sensor_msgs::msg::JointState>(
        input_topic_, 10,
        std::bind(&InverseDynamicsControllerNode::jointStateCallback, this, std::placeholders::_1));
    sub_target_ = create_subscription<boro_interfaces::msg::BowTarget>(
        target_topic_, 10,
        std::bind(&InverseDynamicsControllerNode::targetCallback, this, std::placeholders::_1));

    pub_effort_ = create_publisher<sensor_msgs::msg::JointState>(output_topic_, 10);
    pub_state_ = create_publisher<boro_interfaces::msg::BowState>(state_topic_, 10);

    last_time_ = now();

    active_target_.alpha_mode = boro_interfaces::msg::BowTarget::MODE_POSITION;
    active_target_.h_mode = boro_interfaces::msg::BowTarget::MODE_FORCE;
    active_target_.l_mode = boro_interfaces::msg::BowTarget::MODE_POSITION;
    active_target_.alpha_target = init_a_t_;
    active_target_.alpha_velocity_target = init_ad_t_;
    active_target_.alpha_acceleration_target = init_add_t_;
    active_target_.h_target = init_h_t_;
    active_target_.h_velocity_target = init_hd_t_;
    active_target_.h_acceleration_target = init_hdd_t_;
    active_target_.force_target = init_force_t_;
    active_target_.l_target = init_l_t_;
    active_target_.l_velocity_target = init_ld_t_;
    active_target_.l_acceleration_target = init_ldd_t_;
    active_target_.enable = true;
    active_target_.source_state = "BOOT_DEFAULT";

    RCLCPP_INFO(get_logger(), "InverseDynamicsControllerNode started");
  }

 private:
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr sub_sim_;
  rclcpp::Subscription<boro_interfaces::msg::BowTarget>::SharedPtr sub_target_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr pub_effort_;
  rclcpp::Publisher<boro_interfaces::msg::BowState>::SharedPtr pub_state_;

  pinocchio::Model model_;
  pinocchio::Data data_;
  pinocchio::FrameIndex contact_frame_id_;

  std::string urdf_path_;
  std::string contact_frame_name_;
  std::string input_topic_;
  std::string output_topic_;
  std::string target_topic_;
  std::string state_topic_;

  int normal_axis_index_{};
  int tangent_axis_index_{};

  double init_a_t_;
  double init_ad_t_;
  double init_add_t_;
  double init_h_t_;
  double init_hd_t_;
  double init_hdd_t_;
  double init_l_t_;
  double init_ld_t_;
  double init_ldd_t_;
  double init_force_t_;

  double Mu_{};

  double P_a_free_{};
  double I_a_free_{};
  double D_a_free_{};

  double P_a_contact_{};
  double I_a_contact_{};
  double D_a_contact_{};

  double alpha_integral_;
  double alpha_integral_limit_{};

  double P_h_{};
  double D_h_{};
  double P_l_{};
  double D_l_{};
  double P_vh_{};
  double P_vl_{};
  double I_vl_{};
  double l_velocity_integral_{};
  double l_velocity_integral_limit_{};

  double P_F_{};
  double I_F_{};
  double D_F_{};

  double F_touch_{};
  double F_lose_{};
  double F_integral_limit_{};

  double tau_a_max_{};
  double tau_h_max_{};
  double tau_l_max_{};

  double q1_dd_max_{};
  double q2_dd_max_{};
  double q3_dd_max_{};

  double alpha_ctrl_{};
  double F_ctrl_{0.0};
  double K_contact_ff_{};

  double force_integral_;
  bool target_received_;
  bool in_contact_;
  rclcpp::Time last_time_;
  boro_interfaces::msg::BowTarget active_target_;

  static double clamp(double value, double low, double high) {
    if (value < low) return low;
    if (value > high) return high;
    return value;
  }

  static bool vectorAllFinite(const Eigen::VectorXd& x) {
    for (int i = 0; i < x.size(); ++i) {
      if (!std::isfinite(x[i])) return false;
    }
    return true;
  }

  void initPinocchio(const std::string& urdf_path, const std::string& contact_frame_name) {
    if (urdf_path.empty()) throw std::runtime_error("Parameter 'urdf_path' is empty");

    pinocchio::urdf::buildModel(urdf_path, model_);
    data_ = pinocchio::Data(model_);

    if (!model_.existFrame(contact_frame_name)) {
      throw std::runtime_error("Contact frame not found in URDF: " + contact_frame_name);
    }

    contact_frame_id_ = model_.getFrameId(contact_frame_name);

    if (model_.nq != 3 || model_.nv != 3) {
      throw std::runtime_error("URDF size mismatch, expected 3 DOF robot");
    }
  }

  Eigen::MatrixXd computeM(const Eigen::VectorXd& q) {
    pinocchio::crba(model_, data_, q);
    data_.M.triangularView<Eigen::StrictlyLower>() =
        data_.M.transpose().triangularView<Eigen::StrictlyLower>();
    return data_.M;
  }

  Eigen::VectorXd computeG(const Eigen::VectorXd& q) {
    return pinocchio::computeGeneralizedGravity(model_, data_, q);
  }

  Eigen::VectorXd computeCqd(const Eigen::VectorXd& q, const Eigen::VectorXd& v) {
    const Eigen::VectorXd nle = pinocchio::nonLinearEffects(model_, data_, q, v);
    const Eigen::VectorXd g = pinocchio::computeGeneralizedGravity(model_, data_, q);
    return nle - g;
  }

  void computeContactTerms(const Eigen::VectorXd& q, Eigen::VectorXd& Jn, Eigen::VectorXd& Jt) {
    pinocchio::forwardKinematics(model_, data_, q);
    pinocchio::computeJointJacobians(model_, data_, q);
    pinocchio::updateFramePlacements(model_, data_);

    const pinocchio::SE3& oMf = data_.oMf[contact_frame_id_];
    Eigen::Vector3d normal_world = oMf.rotation().col(normal_axis_index_).normalized();
    Eigen::Vector3d tangent_world = oMf.rotation().col(tangent_axis_index_).normalized();

    pinocchio::Data::Matrix6x J6(6, model_.nv);
    J6.setZero();
    pinocchio::getFrameJacobian(model_, data_, contact_frame_id_, pinocchio::ReferenceFrame::WORLD,
                                J6);

    const Eigen::MatrixXd Jv = J6.bottomRows(3);
    Jn = Jv.transpose() * normal_world;
    Jt = Jv.transpose() * tangent_world;
  }

  void targetCallback(const boro_interfaces::msg::BowTarget::SharedPtr msg) {
    active_target_ = *msg;
    target_received_ = true;
  }

  double computeAdd(double a, double ad, double dt) {
    switch (active_target_.alpha_mode) {
      case boro_interfaces::msg::BowTarget::MODE_OFF:
        alpha_integral_ = 0.0;
        return 0.0;

      case boro_interfaces::msg::BowTarget::MODE_POSITION:
      default: {
        const double P = in_contact_ ? P_a_contact_ : P_a_free_;
        const double I = in_contact_ ? I_a_contact_ : I_a_free_;
        const double D = in_contact_ ? D_a_contact_ : D_a_free_;

        const double a_error = active_target_.alpha_target - a;
        alpha_integral_ += a_error * dt;
        alpha_integral_ = clamp(alpha_integral_, -alpha_integral_limit_, alpha_integral_limit_);

        double qdd = active_target_.alpha_acceleration_target +
                     D * (active_target_.alpha_velocity_target - ad) + P * a_error +
                     I * alpha_integral_;

        return clamp(qdd, -q1_dd_max_, q1_dd_max_);
      }
    }
  }

  double computeHdd(double h, double hd, double F_meas, double dt) {
    switch (active_target_.h_mode) {
      case boro_interfaces::msg::BowTarget::MODE_OFF:
        force_integral_ = 0.0;
        return 0.0;

      case boro_interfaces::msg::BowTarget::MODE_FORCE: {
        F_ctrl_ = alpha_ctrl_ * F_ctrl_ + (1.0 - alpha_ctrl_) * F_meas;
        const double force_error = -active_target_.force_target + F_ctrl_;
        force_integral_ += force_error * dt;
        force_integral_ = clamp(force_integral_, -F_integral_limit_, F_integral_limit_);
        return clamp(P_F_ * force_error + I_F_ * force_integral_ - D_F_ * hd, -q2_dd_max_,
                     q2_dd_max_);
      }

      case boro_interfaces::msg::BowTarget::MODE_VELOCITY:
        force_integral_ = 0.0;
        return clamp(P_vh_ * (active_target_.h_velocity_target - hd), -q2_dd_max_, q2_dd_max_);

      case boro_interfaces::msg::BowTarget::MODE_POSITION:
      default:
        force_integral_ = 0.0;
        return clamp(active_target_.h_acceleration_target +
                         D_h_ * (active_target_.h_velocity_target - hd) +
                         P_h_ * (active_target_.h_target - h),
                     -q2_dd_max_, q2_dd_max_);
    }
  }

  double computeLdd(double l, double ld, double dt) {
    switch (active_target_.l_mode) {
      case boro_interfaces::msg::BowTarget::MODE_OFF:
        l_velocity_integral_ = 0.0;
        return 0.0;

      case boro_interfaces::msg::BowTarget::MODE_VELOCITY: {
        const double l_velocity_error = active_target_.l_velocity_target - ld;
        l_velocity_integral_ += l_velocity_error * dt;
        l_velocity_integral_ =
            clamp(l_velocity_integral_, -l_velocity_integral_limit_, l_velocity_integral_limit_);

        return clamp(P_vl_ * l_velocity_error + I_vl_ * l_velocity_integral_, -q3_dd_max_,
                     q3_dd_max_);
      }

      case boro_interfaces::msg::BowTarget::MODE_POSITION:
      default:
        l_velocity_integral_ = 0.0;
        return clamp(active_target_.l_acceleration_target +
                         D_l_ * (active_target_.l_velocity_target - ld) +
                         P_l_ * (active_target_.l_target - l),
                     -q3_dd_max_, q3_dd_max_);
    }
  }

  void publishState(const rclcpp::Time& now, double a, double h, double l, double ad, double hd,
                    double ld, double F_meas) {
    boro_interfaces::msg::BowState state_msg;
    state_msg.header.stamp = now;
    state_msg.alpha = a;
    state_msg.h = h;
    state_msg.l = l;
    state_msg.alpha_dot = ad;
    state_msg.h_dot = hd;
    state_msg.l_dot = ld;
    state_msg.force = F_meas;
    state_msg.in_contact = in_contact_;
    state_msg.controller_ready = true;
    state_msg.target_received = target_received_;
    state_msg.active_state = active_target_.source_state;
    state_msg.controller_mode = "ID + mixed position/velocity/force";
    pub_state_->publish(state_msg);
  }

  void jointStateCallback(const sensor_msgs::msg::JointState::SharedPtr msg) {
    if (msg->position.size() < 3 || msg->velocity.size() < 3 || msg->effort.size() < 4) {
      RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000,
                           "Input JointState must contain position[3], velocity[3], effort[4]");
      return;
    }

    const rclcpp::Time now_t = now();
    double dt = (now_t - last_time_).seconds();
    if (!std::isfinite(dt) || dt <= 0.0 || dt > 0.1) dt = 0.001;
    last_time_ = now_t;

    const double a = msg->position[0];
    const double h = msg->position[1];
    const double l = msg->position[2];
    const double ad = msg->velocity[0];
    const double hd = msg->velocity[1];
    const double ld = msg->velocity[2];
    const double F_meas = msg->effort[3];

    if (!std::isfinite(a) || !std::isfinite(h) || !std::isfinite(l) || !std::isfinite(ad) ||
        !std::isfinite(hd) || !std::isfinite(ld) || !std::isfinite(F_meas)) {
      RCLCPP_ERROR(get_logger(), "Input contains NaN/Inf");
      return;
    }

    if (!in_contact_ && F_meas >= F_touch_) {
      in_contact_ = true;
      force_integral_ = 0.0;
      alpha_integral_ = 0.0;
    } else if (in_contact_ && F_meas <= F_lose_) {
      in_contact_ = false;
      force_integral_ = 0.0;
      alpha_integral_ = 0.0;
    }

    publishState(now_t, a, h, l, ad, hd, ld, F_meas);

    if (!active_target_.enable || active_target_.emergency_stop) {
      sensor_msgs::msg::JointState out_msg;
      out_msg.header.stamp = now_t;
      out_msg.name = {"joint_a", "joint_h", "joint_l"};
      out_msg.effort = {0.0, 0.0, 0.0};
      pub_effort_->publish(out_msg);
      return;
    }

    Eigen::VectorXd q(3), v(3);
    q << a, h, l;
    v << ad, hd, ld;

    const double q1_dd = computeAdd(a, ad, dt);
    const double q2_dd = computeHdd(h, hd, F_meas, dt);
    const double q3_dd = computeLdd(l, ld, dt);

    Eigen::MatrixXd M = computeM(q);
    Eigen::VectorXd Cqd = computeCqd(q, v);
    Eigen::VectorXd G = computeG(q);
    Eigen::VectorXd Jn(3), Jt(3);
    computeContactTerms(q, Jn, Jt);

    if (!M.allFinite() || !vectorAllFinite(Cqd) || !vectorAllFinite(G) || !vectorAllFinite(Jn) ||
        !vectorAllFinite(Jt)) {
      RCLCPP_ERROR(get_logger(), "NaN/Inf in model terms");
      return;
    }

    const double F_n = std::max(0.0, F_meas);
    const double F_t_contact = Mu_ * F_n;

    Eigen::VectorXd tau_contact = Jn * F_n + Jt * F_t_contact;
    if (!vectorAllFinite(tau_contact)) {
      RCLCPP_ERROR(get_logger(), "tau_contact has NaN/Inf");
      return;
    }

    Eigen::VectorXd qdd_t(3);
    qdd_t << q1_dd, q2_dd, q3_dd;

    if (active_target_.h_mode == boro_interfaces::msg::BowTarget::MODE_FORCE &&
        std::abs(K_contact_ff_) > 1e-12) {
      Eigen::VectorXd qdd_contact_ff = K_contact_ff_ * M.ldlt().solve(tau_contact);

      if (!vectorAllFinite(qdd_contact_ff)) {
        RCLCPP_ERROR(get_logger(), "qdd_contact_ff has NaN/Inf");
        return;
      }

      qdd_t += qdd_contact_ff;
    }

    if (!vectorAllFinite(qdd_t)) {
      RCLCPP_ERROR(get_logger(), "qdd_t has NaN/Inf");
      return;
    }

    Eigen::VectorXd tau = M * qdd_t + Cqd + G - tau_contact;

    if (!vectorAllFinite(tau)) {
      RCLCPP_ERROR(get_logger(), "tau has NaN/Inf");
      return;
    }

    tau(0) = clamp(tau(0), -tau_a_max_, tau_a_max_);
    tau(1) = clamp(tau(1), -tau_h_max_, tau_h_max_);
    tau(2) = clamp(tau(2), -tau_l_max_, tau_l_max_);

    sensor_msgs::msg::JointState out_msg;
    out_msg.header.stamp = now_t;
    out_msg.name = {"joint_a", "joint_h", "joint_l"};
    out_msg.effort = {tau(0), tau(1), tau(2)};
    pub_effort_->publish(out_msg);
  }
};

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  try {
    rclcpp::spin(std::make_shared<InverseDynamicsControllerNode>());
  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what() << std::endl;
  }
  rclcpp::shutdown();
  return 0;
}