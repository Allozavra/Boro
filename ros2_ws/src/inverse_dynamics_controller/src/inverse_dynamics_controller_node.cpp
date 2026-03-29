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

class InverseDynamicsControllerNode : public rclcpp::Node {
 public:
  InverseDynamicsControllerNode()
      : Node("inverse_dynamics_controller_node"),
        force_integral_(0.0),
        l_direction_(1.0),
        in_contact_(false) {
    urdf_path_ = this->declare_parameter<std::string>("urdf_path", "");
    contact_frame_name_ =
        this->declare_parameter<std::string>("contact_frame_name", "bow_contact_frame");

    input_topic_ = this->declare_parameter<std::string>("input_topic", "/sim_ros");
    output_topic_ = this->declare_parameter<std::string>("output_topic", "/ros_sim");

    a_t_ = this->declare_parameter<double>("a_t", 0.0);
    ad_t_ = this->declare_parameter<double>("ad_t", 0.0);
    add_t_ = this->declare_parameter<double>("add_t", 0.0);

    F_t_ = this->declare_parameter<double>("F_t", 0.4);
    hd_t_ = this->declare_parameter<double>("hd_t", 0.4);
    hdd_t_ = this->declare_parameter<double>("hdd_t", 0.4);
    h_sign_ = this->declare_parameter<double>("h_sign", -1.0);

    Mu_ = this->declare_parameter<double>("Mu", 0.3);

    ld_t_abs_ = this->declare_parameter<double>("ld_t", 0.05);
    ldd_t_ = this->declare_parameter<double>("ldd_t", 0.0);
    l_min_ = this->declare_parameter<double>("l_min", -0.2);
    l_max_ = this->declare_parameter<double>("l_max", 0.2);

    P_a_ = this->declare_parameter<double>("P_a", 50.0);
    D_a_ = this->declare_parameter<double>("D_a", 10.0);

    D_ld = this->declare_parameter<double>("D_ld", 10.0);

    P_F_ = this->declare_parameter<double>("P_F", 2.0);
    I_F_ = this->declare_parameter<double>("I_F", 0.0);
    D_F_ = this->declare_parameter<double>("D_F", 8.0);
    D_hd_ = this->declare_parameter<double>("D_hd", 30.0);

    F_touch_ = this->declare_parameter<double>("F_touch", 0.1);
    F_lose_ = this->declare_parameter<double>("F_lose", 0.05);

    q1_dd_max_ = this->declare_parameter<double>("q1_dd_max", 20.0);
    q2_dd_max_ = this->declare_parameter<double>("q2_dd_max", 1.0);
    q3_dd_max_ = this->declare_parameter<double>("q3_dd_max", 5.0);

    tau_a_max_ = this->declare_parameter<double>("tau_a_max", 20.0);
    tau_h_max_ = this->declare_parameter<double>("tau_h_max", 20.0);
    tau_l_max_ = this->declare_parameter<double>("tau_l_max", 20.0);

    force_integral_limit_ = this->declare_parameter<double>("force_integral_limit", 2.0);

    normal_axis_index_ = this->declare_parameter<int>("normal_axis_index", 1);
    tangent_axis_index_ = this->declare_parameter<int>("tangent_axis_index", 0);

    if (normal_axis_index_ < 0 || normal_axis_index_ > 2 || tangent_axis_index_ < 0 ||
        tangent_axis_index_ > 2 || normal_axis_index_ == tangent_axis_index_) {
      throw std::runtime_error(
          "normal_axis_index and tangent_axis_index must be in {0,1,2} and different");
    }

    initPinocchio(urdf_path_, contact_frame_name_);

    if (model_.nq != 3 || model_.nv != 3) {
      throw std::runtime_error("URDF size mismatch: expected nq=3 and nv=3, got nq=" +
                               std::to_string(model_.nq) + ", nv=" + std::to_string(model_.nv));
    }

    sub_ = this->create_subscription<sensor_msgs::msg::JointState>(
        input_topic_, 10,
        std::bind(&InverseDynamicsControllerNode::jointStateCallback, this, std::placeholders::_1));

    pub_ = this->create_publisher<sensor_msgs::msg::JointState>(output_topic_, 10);

    last_time_ = this->now();

    RCLCPP_INFO(this->get_logger(), "InverseDynamicsControllerNode started");
    RCLCPP_INFO(this->get_logger(), "URDF: %s", urdf_path_.c_str());
    RCLCPP_INFO(this->get_logger(), "Contact frame: %s", contact_frame_name_.c_str());
  }

 private:
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr sub_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr pub_;

  pinocchio::Model model_;
  pinocchio::Data data_;
  pinocchio::FrameIndex contact_frame_id_;

  std::string urdf_path_;
  std::string contact_frame_name_;
  std::string input_topic_;
  std::string output_topic_;

  int normal_axis_index_;
  int tangent_axis_index_;

  double a_t_;
  double ad_t_;
  double add_t_;

  double ld_t_abs_;
  double ldd_t_;
  double l_min_;
  double l_max_;

  double F_t_;
  double Mu_;

  double P_a_;
  double D_a_;
  double D_ld;
  double P_F_;
  double I_F_;
  double D_F_;
  double D_hd_;

  double hdd_t_;
  double hd_t_;
  double h_sign_;
  double F_touch_;
  double F_lose_;

  double q1_dd_max_;
  double q2_dd_max_;
  double q3_dd_max_;

  double tau_a_max_;
  double tau_h_max_;
  double tau_l_max_;
  double force_integral_limit_;

  double force_integral_;
  double l_direction_;
  bool in_contact_;
  rclcpp::Time last_time_;

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
    if (urdf_path.empty()) {
      throw std::runtime_error("Parameter 'urdf_path' is empty");
    }

    pinocchio::urdf::buildModel(urdf_path, model_);
    data_ = pinocchio::Data(model_);

    if (!model_.existFrame(contact_frame_name)) {
      throw std::runtime_error("Contact frame not found in URDF: " + contact_frame_name);
    }

    contact_frame_id_ = model_.getFrameId(contact_frame_name);
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
    const Eigen::Matrix3d& R = oMf.rotation();

    Eigen::Vector3d normal_world = R.col(normal_axis_index_);
    Eigen::Vector3d tangent_world = R.col(tangent_axis_index_);

    if (normal_world.norm() < 1e-12 || tangent_world.norm() < 1e-12) {
      throw std::runtime_error("Contact frame axis has near-zero norm");
    }

    normal_world.normalize();
    tangent_world.normalize();

    pinocchio::Data::Matrix6x J6(6, model_.nv);
    J6.setZero();

    pinocchio::getFrameJacobian(model_, data_, contact_frame_id_, pinocchio::ReferenceFrame::WORLD,
                                J6);

    const Eigen::MatrixXd Jv = J6.bottomRows(3);

    Jn = Jv.transpose() * normal_world;
    Jt = Jv.transpose() * tangent_world;
  }

  void jointStateCallback(const sensor_msgs::msg::JointState::SharedPtr msg) {
    if (msg->position.size() < 3 || msg->velocity.size() < 3 || msg->effort.size() < 4) {
      RCLCPP_WARN_THROTTLE(
          this->get_logger(), *this->get_clock(), 1000,
          "Input JointState must contain at least position[3], velocity[3], effort[4]");
      return;
    }

    const rclcpp::Time now = this->now();
    double dt = (now - last_time_).seconds();
    if (!std::isfinite(dt) || dt <= 0.0 || dt > 0.1) {
      dt = 0.001;
    }
    last_time_ = now;

    const double a = msg->position[0];
    const double h = msg->position[1];
    const double l = msg->position[2];

    const double ad = msg->velocity[0];
    const double hd = msg->velocity[1];
    const double ld = msg->velocity[2];

    const double F_meas = msg->effort[3];

    if (!std::isfinite(a) || !std::isfinite(h) || !std::isfinite(l) || !std::isfinite(ad) ||
        !std::isfinite(hd) || !std::isfinite(ld) || !std::isfinite(F_meas)) {
      RCLCPP_ERROR(this->get_logger(), "Input contains NaN/Inf");
      return;
    }

    // Contact state with hysteresis
    if (!in_contact_ && F_meas >= F_touch_) {
      in_contact_ = true;
      force_integral_ = 0.0;
      RCLCPP_INFO(this->get_logger(), "Contact detected");
    } else if (in_contact_ && F_meas <= F_lose_) {
      in_contact_ = false;
      force_integral_ = 0.0;
      RCLCPP_WARN(this->get_logger(), "Contact lost");
    }

    // Reverse bow direction at stroke limits
    if (l >= l_max_) {
      l_direction_ = -1.0;
      // RCLCPP_INFO(this->get_logger(), "Direction 1");
    } else if (l <= l_min_) {
      l_direction_ = 1.0;
      // RCLCPP_INFO(this->get_logger(), "Direction -1");
    }

    Eigen::VectorXd q(3);
    q << a, h, l;

    Eigen::VectorXd v(3);
    v << ad, hd, ld;

    // Desired accelerations
    double q1_dd = add_t_ + D_a_ * (ad_t_ - ad) + P_a_ * (a_t_ - a);

    // q1_dd = clamp(q1_dd, -q1_dd_max_, q1_dd_max_);

    double q2_dd = 0.0;
    if (in_contact_) {
      const double force_error = -F_t_ + F_meas;
      force_integral_ += force_error * dt;
      // force_integral_ = clamp(force_integral_, -force_integral_limit_, force_integral_limit_);
      q2_dd = P_F_ * force_error + I_F_ * force_integral_ - D_F_ * hd;
    } else {
      force_integral_ = 0.0;
      q2_dd = h_sign_ * hdd_t_ + D_hd_ * (hd_t_ - hd);
    }

    // q2_dd = clamp(q2_dd, -q2_dd_max_, q2_dd_max_);

    const double ld_t = l_direction_ * ld_t_abs_;
    double q3_dd = ldd_t_ + D_ld * (ld_t - ld);
    // q3_dd = clamp(q3_dd, -q3_dd_max_, q3_dd_max_);

    //!!!
    // q1_dd = 0;
    // q2_dd = 0;
    // q3_dd = 0;
    // if (l >= l_max_) {
    //   q3_dd = 5.0;
    //   // RCLCPP_INFO(this->get_logger(), "Direction 1");
    // } else if (l <= l_min_) {
    //   q3_dd = -5.0;
    //   // RCLCPP_INFO(this->get_logger(), "Direction -1");
    // }

    Eigen::VectorXd qdd_t(3);
    qdd_t << q1_dd, q2_dd, q3_dd;

    Eigen::MatrixXd M = computeM(q);
    Eigen::VectorXd Cqd = computeCqd(q, v);
    Eigen::VectorXd G = computeG(q);
    Eigen::VectorXd Jn(3), Jt(3);
    computeContactTerms(q, Jn, Jt);

    if (!M.allFinite() || !vectorAllFinite(Cqd) || !vectorAllFinite(G) || !vectorAllFinite(Jn) ||
        !vectorAllFinite(Jt) || !vectorAllFinite(qdd_t)) {
      RCLCPP_ERROR(this->get_logger(), "NaN/Inf in model terms");
      return;
    }

    const double F_n = F_meas;
    const double F_t_contact = Mu_ * F_n;

    Eigen::VectorXd tau = M * qdd_t + Cqd + G - Jn * F_n - Jt * F_t_contact;

    if (!vectorAllFinite(tau)) {
      RCLCPP_ERROR(this->get_logger(),
                   "tau has NaN/Inf. q=[%.6f %.6f %.6f], v=[%.6f %.6f %.6f], F=%.6f", a, h, l, ad,
                   hd, ld, F_meas);
      return;
    }

    tau(0) = clamp(tau(0), -tau_a_max_, tau_a_max_);
    tau(1) = clamp(tau(1), -tau_h_max_, tau_h_max_);
    tau(2) = clamp(tau(2), -tau_l_max_, tau_l_max_);

    sensor_msgs::msg::JointState out_msg;
    out_msg.header.stamp = now;
    out_msg.name = {"joint_a", "joint_h", "joint_l"};
    out_msg.effort = {tau(0), tau(1), tau(2)};

    pub_->publish(out_msg);
    // RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 200,
    //                      "dir=%.1f l_t_d=%.3f ld=%.3f q3_dd=%.3f | Mqdd3=%.3f C3=%.3f G3=%.3f
    //                      " "JnF3=%.3f JtF3=%.3f tau3=%.3f", l_direction_, l_t_d, ld, q3_dd, (M
    //                      * qdd_t)(2), Cqd(2), G(2), (Jn * F_n)(2), (Jt * F_t_contact)(2),
    //                      tau(2));
  }
};

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);

  try {
    auto node = std::make_shared<InverseDynamicsControllerNode>();
    rclcpp::spin(node);
  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what() << std::endl;
  }

  rclcpp::shutdown();
  return 0;
}