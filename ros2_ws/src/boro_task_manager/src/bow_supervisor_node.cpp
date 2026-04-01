#include <algorithm>
#include <cmath>
#include <map>
#include <memory>
#include <string>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>

#include "bow_interfaces/msg/bow_event.hpp"
#include "bow_interfaces/msg/bow_state.hpp"
#include "bow_interfaces/msg/bow_target.hpp"
#include "bow_interfaces/msg/supervisor_command.hpp"

class BowSupervisorNode : public rclcpp::Node {
 public:
  BowSupervisorNode() : Node("bow_supervisor_node") {
    input_state_topic_ = declare_parameter<std::string>("input_state_topic", "/bow/state");
    command_topic_ = declare_parameter<std::string>("command_topic", "/bow/command");
    target_topic_ = declare_parameter<std::string>("target_topic", "/bow/target");
    event_topic_ = declare_parameter<std::string>("event_topic", "/bow/event");

    loop_hz_ = declare_parameter<double>("loop_hz", 200.0);
    startup_wait_sec_ = declare_parameter<double>("startup_wait_sec", 1.0);
    home_alpha_ = declare_parameter<double>("home_alpha", 0.0);
    home_h_ = declare_parameter<double>("home_h", 0.03);
    home_l_ = declare_parameter<double>("home_l", 0.0);
    safe_h_ = declare_parameter<double>("safe_h", 0.02);
    start_l_ = declare_parameter<double>("start_l", -0.16);
    finish_l_ = declare_parameter<double>("finish_l", 0.16);
    home_tol_alpha_ = declare_parameter<double>("home_tol_alpha", 0.02);
    home_tol_h_ = declare_parameter<double>("home_tol_h", 0.003);
    home_tol_l_ = declare_parameter<double>("home_tol_l", 0.003);
    force_tol_ = declare_parameter<double>("force_tol", 0.05);
    force_stable_time_sec_ = declare_parameter<double>("force_stable_time_sec", 0.2);
    force_contact_on_ = declare_parameter<double>("force_contact_on", 0.10);
    force_contact_off_ = declare_parameter<double>("force_contact_off", 0.05);
    force_max_ = declare_parameter<double>("force_max", 2.0);
    lower_timeout_sec_ = declare_parameter<double>("lower_timeout_sec", 3.0);
    raise_timeout_sec_ = declare_parameter<double>("raise_timeout_sec", 3.0);
    home_timeout_sec_ = declare_parameter<double>("home_timeout_sec", 3.0);
    h_min_ = declare_parameter<double>("h_min", -0.05);
    h_max_ = declare_parameter<double>("h_max", 0.05);
    l_min_ = declare_parameter<double>("l_min", -0.20);
    l_max_ = declare_parameter<double>("l_max", 0.20);
    approach_velocity_default_ = declare_parameter<double>("approach_velocity_default", -0.01);
    raise_velocity_default_ = declare_parameter<double>("raise_velocity_default", 0.02);
    stroke_velocity_default_ = declare_parameter<double>("stroke_velocity_default", 0.05);
    desired_force_default_ = declare_parameter<double>("desired_force_default", 0.4);

    string_alpha_["G"] = declare_parameter<double>("strings.G", -0.26);
    string_alpha_["D"] = declare_parameter<double>("strings.D", -0.09);
    string_alpha_["A"] = declare_parameter<double>("strings.A", 0.09);
    string_alpha_["E"] = declare_parameter<double>("strings.E", 0.26);

    state_sub_ = create_subscription<bow_interfaces::msg::BowState>(
        input_state_topic_, 10,
        std::bind(&BowSupervisorNode::stateCallback, this, std::placeholders::_1));
    cmd_sub_ = create_subscription<bow_interfaces::msg::SupervisorCommand>(
        command_topic_, 10,
        std::bind(&BowSupervisorNode::commandCallback, this, std::placeholders::_1));

    target_pub_ = create_publisher<bow_interfaces::msg::BowTarget>(target_topic_, 10);
    event_pub_ = create_publisher<bow_interfaces::msg::BowEvent>(event_topic_, 10);

    timer_ = create_wall_timer(
        std::chrono::duration<double>(1.0 / std::max(loop_hz_, 1.0)),
        std::bind(&BowSupervisorNode::update, this));

    transitionTo(TaskState::STARTUP, "startup");
    RCLCPP_INFO(get_logger(), "BowSupervisorNode started");
  }

 private:
  enum class TaskState {
    STARTUP,
    HOME,
    WAIT_STRING_SELECT,
    LOWER_BOW,
    HOLD_FORCE,
    BOW_STROKE,
    RAISE_BOW,
    RETURN_HOME,
    FINISH,
    ERROR_HOLD
  };

  struct PendingCommand {
    uint8_t command{bow_interfaces::msg::SupervisorCommand::CMD_NONE};
    std::string selected_string{"D"};
    double desired_alpha{0.0};
    double desired_force{0.4};
    double bow_velocity{0.05};
    double bow_range_min{-0.16};
    double bow_range_max{0.16};
    double h_approach_velocity{-0.01};
    double h_raise_velocity{0.02};
    bool auto_continue{true};
  } cmd_;

  rclcpp::Subscription<bow_interfaces::msg::BowState>::SharedPtr state_sub_;
  rclcpp::Subscription<bow_interfaces::msg::SupervisorCommand>::SharedPtr cmd_sub_;
  rclcpp::Publisher<bow_interfaces::msg::BowTarget>::SharedPtr target_pub_;
  rclcpp::Publisher<bow_interfaces::msg::BowEvent>::SharedPtr event_pub_;
  rclcpp::TimerBase::SharedPtr timer_;

  std::string input_state_topic_;
  std::string command_topic_;
  std::string target_topic_;
  std::string event_topic_;

  double loop_hz_{};
  double startup_wait_sec_{};
  double home_alpha_{};
  double home_h_{};
  double home_l_{};
  double safe_h_{};
  double start_l_{};
  double finish_l_{};
  double home_tol_alpha_{};
  double home_tol_h_{};
  double home_tol_l_{};
  double force_tol_{};
  double force_stable_time_sec_{};
  double force_contact_on_{};
  double force_contact_off_{};
  double force_max_{};
  double lower_timeout_sec_{};
  double raise_timeout_sec_{};
  double home_timeout_sec_{};
  double h_min_{};
  double h_max_{};
  double l_min_{};
  double l_max_{};
  double approach_velocity_default_{};
  double raise_velocity_default_{};
  double stroke_velocity_default_{};
  double desired_force_default_{};
  std::map<std::string, double> string_alpha_;

  bow_interfaces::msg::BowState latest_state_;
  bool state_received_{false};
  bool event_force_stable_sent_{false};
  bool event_contact_sent_{false};
  rclcpp::Time state_enter_time_{0, 0, RCL_ROS_TIME};
  rclcpp::Time force_stable_start_{0, 0, RCL_ROS_TIME};
  TaskState state_{TaskState::STARTUP};
  uint32_t error_code_{bow_interfaces::msg::BowEvent::NONE};
  std::string error_text_;

  static double clamp(double v, double lo, double hi) { return std::min(std::max(v, lo), hi); }

  std::string stateName(TaskState s) const {
    switch (s) {
      case TaskState::STARTUP:
        return "STARTUP";
      case TaskState::HOME:
        return "HOME";
      case TaskState::WAIT_STRING_SELECT:
        return "WAIT_STRING_SELECT";
      case TaskState::LOWER_BOW:
        return "LOWER_BOW";
      case TaskState::HOLD_FORCE:
        return "HOLD_FORCE";
      case TaskState::BOW_STROKE:
        return "BOW_STROKE";
      case TaskState::RAISE_BOW:
        return "RAISE_BOW";
      case TaskState::RETURN_HOME:
        return "RETURN_HOME";
      case TaskState::FINISH:
        return "FINISH";
      case TaskState::ERROR_HOLD:
        return "ERROR_HOLD";
    }
    return "UNKNOWN";
  }

  void stateCallback(const bow_interfaces::msg::BowState::SharedPtr msg) {
    latest_state_ = *msg;
    state_received_ = true;
  }

  void commandCallback(const bow_interfaces::msg::SupervisorCommand::SharedPtr msg) {
    cmd_.command = msg->command;
    if (!msg->selected_string.empty()) cmd_.selected_string = msg->selected_string;
    cmd_.desired_alpha = msg->desired_alpha;
    cmd_.desired_force = msg->desired_force > 0.0 ? msg->desired_force : desired_force_default_;
    cmd_.bow_velocity = std::abs(msg->bow_velocity) > 1e-9 ? msg->bow_velocity : stroke_velocity_default_;
    cmd_.bow_range_min = msg->bow_range_min;
    cmd_.bow_range_max = msg->bow_range_max;
    cmd_.h_approach_velocity =
        std::abs(msg->h_approach_velocity) > 1e-9 ? msg->h_approach_velocity : approach_velocity_default_;
    cmd_.h_raise_velocity =
        std::abs(msg->h_raise_velocity) > 1e-9 ? msg->h_raise_velocity : raise_velocity_default_;
    cmd_.auto_continue = msg->auto_continue;
  }

  void transitionTo(TaskState next, const std::string& why) {
    state_ = next;
    state_enter_time_ = now();
    force_stable_start_ = rclcpp::Time(0, 0, get_clock()->get_clock_type());
    event_force_stable_sent_ = false;
    event_contact_sent_ = false;
    RCLCPP_INFO(get_logger(), "Transition -> %s (%s)", stateName(state_).c_str(), why.c_str());
  }

  void publishEvent(uint32_t code, const std::string& description, bool is_error = false) {
    bow_interfaces::msg::BowEvent msg;
    msg.header.stamp = now();
    msg.event_code = code;
    msg.state_name = stateName(state_);
    msg.description = description;
    msg.level_error = is_error;
    event_pub_->publish(msg);
  }

  bool near(double x, double ref, double tol) const { return std::abs(x - ref) <= tol; }

  bool homeReached() const {
    return state_received_ && near(latest_state_.alpha, home_alpha_, home_tol_alpha_) &&
           near(latest_state_.h, home_h_, home_tol_h_) && near(latest_state_.l, home_l_, home_tol_l_);
  }

  double selectedAlpha() const {
    auto it = string_alpha_.find(cmd_.selected_string);
    if (it != string_alpha_.end()) return it->second;
    return cmd_.desired_alpha;
  }

  bow_interfaces::msg::BowTarget makeBaseTarget() const {
    bow_interfaces::msg::BowTarget msg;
    msg.header.stamp = now();
    msg.enable = true;
    msg.emergency_stop = false;
    msg.source_state = stateName(state_);
    msg.alpha_mode = bow_interfaces::msg::BowTarget::MODE_POSITION;
    msg.h_mode = bow_interfaces::msg::BowTarget::MODE_POSITION;
    msg.l_mode = bow_interfaces::msg::BowTarget::MODE_POSITION;
    msg.alpha_target = home_alpha_;
    msg.h_target = home_h_;
    msg.l_target = home_l_;
    return msg;
  }

  void fail(uint32_t error_code, const std::string& error_text) {
    error_code_ = error_code;
    error_text_ = error_text;
    publishEvent(error_code, error_text, true);
    transitionTo(TaskState::ERROR_HOLD, error_text);
  }

  void update() {
    if (!state_received_) return;

    if (latest_state_.force > force_max_) {
      fail(bow_interfaces::msg::BowEvent::FORCE_OVERLIMIT, "normal force above limit");
      return;
    }
    if (latest_state_.h < h_min_ || latest_state_.h > h_max_ || latest_state_.l < l_min_ ||
        latest_state_.l > l_max_) {
      fail(bow_interfaces::msg::BowEvent::LIMIT_REACHED, "joint limit reached");
      return;
    }

    const double elapsed = (now() - state_enter_time_).seconds();
    auto target = makeBaseTarget();

    switch (state_) {
      case TaskState::STARTUP:
        if (elapsed >= startup_wait_sec_) {
          transitionTo(TaskState::HOME, "startup timer elapsed");
        }
        break;

      case TaskState::HOME:
        if (homeReached()) publishEvent(bow_interfaces::msg::BowEvent::HOME_REACHED, "home reached");
        if (cmd_.command == bow_interfaces::msg::SupervisorCommand::CMD_SELECT_STRING) {
          transitionTo(TaskState::WAIT_STRING_SELECT, "string select command");
          cmd_.command = bow_interfaces::msg::SupervisorCommand::CMD_NONE;
        } else if (cmd_.command == bow_interfaces::msg::SupervisorCommand::CMD_FINISH) {
          transitionTo(TaskState::FINISH, "finish command");
          cmd_.command = bow_interfaces::msg::SupervisorCommand::CMD_NONE;
        } else if (elapsed > home_timeout_sec_ && !homeReached()) {
          fail(bow_interfaces::msg::BowEvent::TIMEOUT, "home timeout");
          return;
        }
        break;

      case TaskState::WAIT_STRING_SELECT:
        target.alpha_target = selectedAlpha();
        target.h_target = safe_h_;
        target.l_target = start_l_;
        if (near(latest_state_.alpha, target.alpha_target, home_tol_alpha_) &&
            near(latest_state_.h, safe_h_, home_tol_h_) && near(latest_state_.l, start_l_, 0.01)) {
          publishEvent(bow_interfaces::msg::BowEvent::STRING_READY, "selected string ready");
          if (cmd_.command == bow_interfaces::msg::SupervisorCommand::CMD_LOWER_BOW || cmd_.auto_continue) {
            transitionTo(TaskState::LOWER_BOW, "start lowering");
            cmd_.command = bow_interfaces::msg::SupervisorCommand::CMD_NONE;
          }
        }
        break;

      case TaskState::LOWER_BOW:
        target.alpha_target = selectedAlpha();
        target.l_target = start_l_;
        target.h_mode = bow_interfaces::msg::BowTarget::MODE_VELOCITY;
        target.h_velocity_target = cmd_.h_approach_velocity;
        if (latest_state_.force >= force_contact_on_ || latest_state_.in_contact) {
          if (!event_contact_sent_) {
            event_contact_sent_ = true;
            publishEvent(bow_interfaces::msg::BowEvent::CONTACT_DETECTED, "contact detected");
          }
          transitionTo(TaskState::HOLD_FORCE, "contact detected");
        } else if (elapsed > lower_timeout_sec_) {
          fail(bow_interfaces::msg::BowEvent::NO_CONTACT, "contact not detected in time");
          return;
        }
        break;

      case TaskState::HOLD_FORCE:
        target.alpha_target = selectedAlpha();
        target.l_target = start_l_;
        target.h_mode = bow_interfaces::msg::BowTarget::MODE_FORCE;
        target.force_target = cmd_.desired_force;
        if (latest_state_.force <= force_contact_off_) {
          fail(bow_interfaces::msg::BowEvent::CONTACT_LOST, "contact lost before stroke");
          return;
        }
        if (std::abs(latest_state_.force - cmd_.desired_force) <= force_tol_) {
          if (force_stable_start_.nanoseconds() == 0) force_stable_start_ = now();
          if (!event_force_stable_sent_ && (now() - force_stable_start_).seconds() >= force_stable_time_sec_) {
            publishEvent(bow_interfaces::msg::BowEvent::FORCE_STABLE, "force stabilized");
            event_force_stable_sent_ = true;
            if (cmd_.command == bow_interfaces::msg::SupervisorCommand::CMD_START_STROKE || cmd_.auto_continue) {
              transitionTo(TaskState::BOW_STROKE, "begin stroke");
              cmd_.command = bow_interfaces::msg::SupervisorCommand::CMD_NONE;
            }
          }
        } else {
          force_stable_start_ = rclcpp::Time(0, 0, get_clock()->get_clock_type());
        }
        break;

      case TaskState::BOW_STROKE: {
        target.alpha_target = selectedAlpha();
        target.h_mode = bow_interfaces::msg::BowTarget::MODE_FORCE;
        target.force_target = cmd_.desired_force;
        target.l_mode = bow_interfaces::msg::BowTarget::MODE_VELOCITY;
        const double v = cmd_.bow_velocity;
        const double l_target_end = v >= 0.0 ? cmd_.bow_range_max : cmd_.bow_range_min;
        target.l_velocity_target = v;
        if (latest_state_.force <= force_contact_off_) {
          fail(bow_interfaces::msg::BowEvent::CONTACT_LOST, "contact lost during stroke");
          return;
        }
        const bool reached_end = (v >= 0.0 && latest_state_.l >= l_target_end) ||
                                 (v < 0.0 && latest_state_.l <= l_target_end);
        if (reached_end) {
          publishEvent(bow_interfaces::msg::BowEvent::STROKE_DONE, "stroke done");
          transitionTo(TaskState::RAISE_BOW, "stroke finished");
        }
      } break;

      case TaskState::RAISE_BOW:
        target.alpha_target = selectedAlpha();
        target.l_target = latest_state_.l;
        target.h_mode = bow_interfaces::msg::BowTarget::MODE_VELOCITY;
        target.h_velocity_target = cmd_.h_raise_velocity;
        if (latest_state_.h >= safe_h_) {
          publishEvent(bow_interfaces::msg::BowEvent::BOW_RAISED, "bow raised");
          transitionTo(TaskState::RETURN_HOME, "safe height reached");
        } else if (elapsed > raise_timeout_sec_) {
          fail(bow_interfaces::msg::BowEvent::TIMEOUT, "raise timeout");
          return;
        }
        break;

      case TaskState::RETURN_HOME:
        target.alpha_target = home_alpha_;
        target.h_target = home_h_;
        target.l_target = home_l_;
        if (homeReached()) {
          transitionTo(TaskState::WAIT_STRING_SELECT, "home after cycle");
        } else if (elapsed > home_timeout_sec_) {
          fail(bow_interfaces::msg::BowEvent::TIMEOUT, "return home timeout");
          return;
        }
        break;

      case TaskState::FINISH:
        target.alpha_target = home_alpha_;
        target.h_target = home_h_;
        target.l_target = home_l_;
        if (homeReached()) publishEvent(bow_interfaces::msg::BowEvent::FINISHED, "cycle finished");
        target.enable = false;
        break;

      case TaskState::ERROR_HOLD:
        target.alpha_target = latest_state_.alpha;
        target.h_target = clamp(latest_state_.h + 0.002, h_min_, h_max_);
        target.l_target = latest_state_.l;
        target.enable = true;
        if (cmd_.command == bow_interfaces::msg::SupervisorCommand::CMD_RESET_ERROR) {
          error_code_ = bow_interfaces::msg::BowEvent::NONE;
          error_text_.clear();
          transitionTo(TaskState::HOME, "error reset");
          cmd_.command = bow_interfaces::msg::SupervisorCommand::CMD_NONE;
        }
        break;
    }

    target_pub_->publish(target);
  }
};

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<BowSupervisorNode>());
  rclcpp::shutdown();
  return 0;
}
