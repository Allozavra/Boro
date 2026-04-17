#include <algorithm>
#include <cmath>
#include <map>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <string>

#include "boro_interfaces/msg/bow_event.hpp"
#include "boro_interfaces/msg/bow_state.hpp"
#include "boro_interfaces/msg/bow_target.hpp"
#include "boro_interfaces/msg/task_manager_command.hpp"

class BoroTaskManagerNode : public rclcpp::Node {
 public:
  BoroTaskManagerNode() : Node("boro_task_manager_node") {
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

    home_tol_alpha_ = declare_parameter<double>("home_tol_alpha", 0.02);
    home_tol_h_ = declare_parameter<double>("home_tol_h", 0.003);
    home_tol_l_ = declare_parameter<double>("home_tol_l", 0.003);

    angle_tol_ = declare_parameter<double>("angle_tol", 0.02);
    pose_tol_l_ = declare_parameter<double>("pose_tol_l", 0.01);

    force_tol_ = declare_parameter<double>("force_tol", 0.05);

    force_contact_on_ = declare_parameter<double>("force_contact_on", 0.10);
    force_contact_off_ = declare_parameter<double>("force_contact_off", 0.03);
    force_h_offset_ = declare_parameter<double>("force_h_offset", 0.001);

    contact_reached_hold_sec_ = declare_parameter<double>("contact_reached_hold_sec", 0.015);
    contact_lost_hold_sec_ = declare_parameter<double>("contact_lost_hold_sec", 0.040);

    contact_settle_time_sec_ = declare_parameter<double>("contact_settle_time_sec", 0.05);
    contact_h_tolerance_ = declare_parameter<double>("contact_h_tolerance", 0.001);
    contact_ready_force_min_ = declare_parameter<double>("contact_ready_force_min", 0.03);

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

    state_sub_ = create_subscription<boro_interfaces::msg::BowState>(
        input_state_topic_, 10,
        std::bind(&BoroTaskManagerNode::stateCallback, this, std::placeholders::_1));

    cmd_sub_ = create_subscription<boro_interfaces::msg::TaskManagerCommand>(
        command_topic_, 10,
        std::bind(&BoroTaskManagerNode::commandCallback, this, std::placeholders::_1));

    target_pub_ = create_publisher<boro_interfaces::msg::BowTarget>(target_topic_, 10);
    event_pub_ = create_publisher<boro_interfaces::msg::BowEvent>(event_topic_, 10);

    timer_ = create_wall_timer(std::chrono::duration<double>(1.0 / std::max(loop_hz_, 1.0)),
                               std::bind(&BoroTaskManagerNode::update, this));

    transitionTo(TaskState::STARTUP, "startup");
    RCLCPP_INFO(get_logger(), "BoroTaskManagerNode started");
  }

 private:
  enum class TaskState {
    STARTUP,
    HOME,
    CONFIG_SELECTED,
    ROTATING,
    LOWER_BOW,
    CONTACT_SETTLE,
    HOLD_FORCE,
    BOWING,
    RAISE_BOW,
    RETURN_HOME,
    FINISH,
    ERROR_HOLD,
    TESTING
  };

  struct PendingCommand {
    uint8_t command{boro_interfaces::msg::TaskManagerCommand::CMD_NONE};
    std::string selected_string{"E"};
    double desired_alpha{0.0};
    double desired_force{0.4};
    double bow_velocity{0.05};
    double bow_range_min{-0.16};
    double bow_range_max{0.16};
    double h_approach_velocity{-0.01};
    double h_raise_velocity{0.02};
    bool auto_continue{true};
  } cmd_;

  rclcpp::Subscription<boro_interfaces::msg::BowState>::SharedPtr state_sub_;
  rclcpp::Subscription<boro_interfaces::msg::TaskManagerCommand>::SharedPtr cmd_sub_;
  rclcpp::Publisher<boro_interfaces::msg::BowTarget>::SharedPtr target_pub_;
  rclcpp::Publisher<boro_interfaces::msg::BowEvent>::SharedPtr event_pub_;
  rclcpp::TimerBase::SharedPtr timer_;

  std::string input_state_topic_;
  std::string command_topic_;
  std::string target_topic_;
  std::string event_topic_;

  bool all_auto_continue_{true};  // Flag

  double loop_hz_{};
  double startup_wait_sec_{};

  double home_alpha_{};
  double home_h_{};
  double home_l_{};

  double safe_h_{};
  double start_l_{};

  double home_tol_alpha_{};
  double home_tol_h_{};
  double home_tol_l_{};
  double angle_tol_{};
  double pose_tol_l_{};

  double force_tol_{};
  double force_contact_on_{};
  double force_contact_off_{};
  double force_h_offset_{};

  double contact_reached_hold_sec_{};
  double contact_lost_hold_sec_{};

  double contact_settle_time_sec_{};
  double contact_h_tolerance_{};
  double contact_ready_force_min_{};

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

  boro_interfaces::msg::BowState latest_state_;
  bool state_received_{false};

  TaskState state_{TaskState::STARTUP};
  rclcpp::Time state_enter_time_{0, 0, RCL_ROS_TIME};

  bool ready_sent_{false};
  bool home_event_sent_{false};
  bool config_selected_event_sent_{false};
  bool angle_reached_event_sent_{false};
  bool contact_event_sent_{false};
  bool contact_ready_event_sent_{false};
  bool config_played_event_sent_{false};
  bool bow_raised_event_sent_{false};
  bool shutdown_selected_event_sent_{false};

  double contact_reached_accum_sec_{0.0};
  double contact_lost_accum_sec_{0.0};

  double contact_h_{0.0};

  uint32_t error_code_{boro_interfaces::msg::BowEvent::NONE};
  std::string error_text_;

  static double clamp(double v, double lo, double hi) { return std::min(std::max(v, lo), hi); }

  std::string stateName(TaskState s) const {
    switch (s) {
      case TaskState::STARTUP:
        return "STARTUP";
      case TaskState::HOME:
        return "HOME";
      case TaskState::CONFIG_SELECTED:
        return "CONFIG_SELECTED";
      case TaskState::ROTATING:
        return "ROTATING";
      case TaskState::LOWER_BOW:
        return "LOWER_BOW";
      case TaskState::CONTACT_SETTLE:
        return "CONTACT_SETTLE";
      case TaskState::HOLD_FORCE:
        return "HOLD_FORCE";
      case TaskState::BOWING:
        return "BOWING";
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

  void stateCallback(const boro_interfaces::msg::BowState::SharedPtr msg) {
    latest_state_ = *msg;
    state_received_ = true;
  }

  void commandCallback(const boro_interfaces::msg::TaskManagerCommand::SharedPtr msg) {
    cmd_.command = msg->command;

    if (!msg->selected_string.empty()) {
      cmd_.selected_string = msg->selected_string;
    }

    cmd_.desired_alpha = msg->desired_alpha;
    cmd_.desired_force = (msg->desired_force > 0.0) ? msg->desired_force : desired_force_default_;
    cmd_.bow_velocity =
        (std::abs(msg->bow_velocity) > 1e-9) ? msg->bow_velocity : stroke_velocity_default_;
    cmd_.bow_range_min = msg->bow_range_min;
    cmd_.bow_range_max = msg->bow_range_max;
    cmd_.h_approach_velocity = (std::abs(msg->h_approach_velocity) > 1e-9)
                                   ? msg->h_approach_velocity
                                   : approach_velocity_default_;
    cmd_.h_raise_velocity =
        (std::abs(msg->h_raise_velocity) > 1e-9) ? msg->h_raise_velocity : raise_velocity_default_;
    cmd_.auto_continue = msg->auto_continue;
  }

  void resetTransientFlags() {
    angle_reached_event_sent_ = false;
    contact_event_sent_ = false;
    contact_ready_event_sent_ = false;
    config_played_event_sent_ = false;
    bow_raised_event_sent_ = false;

    contact_reached_accum_sec_ = 0.0;
    contact_lost_accum_sec_ = 0.0;
  }

  void transitionTo(TaskState next, const std::string& why) {
    state_ = next;
    state_enter_time_ = now();
    resetTransientFlags();
    RCLCPP_INFO(get_logger(), "Transition -> %s (%s)", stateName(state_).c_str(), why.c_str());
  }

  void publishEvent(uint32_t code, const std::string& description, bool is_error = false) {
    boro_interfaces::msg::BowEvent msg;
    msg.header.stamp = now();
    msg.event_code = code;
    msg.state_name = stateName(state_);
    msg.description = description;
    msg.level_error = is_error;
    event_pub_->publish(msg);
  }

  void fail(uint32_t error_code, const std::string& error_text) {
    error_code_ = error_code;
    error_text_ = error_text;
    publishEvent(error_code, error_text, true);
    transitionTo(TaskState::ERROR_HOLD, error_text);
  }

  bool near(double x, double ref, double tol) const { return std::abs(x - ref) <= tol; }

  bool homeReached() const {
    return state_received_ && near(latest_state_.alpha, home_alpha_, home_tol_alpha_) &&
           near(latest_state_.h, home_h_, home_tol_h_) &&
           near(latest_state_.l, home_l_, home_tol_l_);
  }

  double selectedAlpha() const {
    auto it = string_alpha_.find(cmd_.selected_string);
    if (it != string_alpha_.end()) {
      return it->second;
    }
    return cmd_.desired_alpha;
  }

  boro_interfaces::msg::BowTarget makeBaseTarget() const {
    boro_interfaces::msg::BowTarget msg;
    msg.header.stamp = now();
    msg.enable = true;
    msg.emergency_stop = false;
    msg.source_state = stateName(state_);

    msg.alpha_mode = boro_interfaces::msg::BowTarget::MODE_POSITION;
    msg.h_mode = boro_interfaces::msg::BowTarget::MODE_POSITION;
    msg.l_mode = boro_interfaces::msg::BowTarget::MODE_POSITION;

    msg.alpha_target = home_alpha_;
    msg.h_target = home_h_;
    msg.l_target = home_l_;

    msg.alpha_velocity_target = 0.0;
    msg.alpha_acceleration_target = 0.0;
    msg.h_velocity_target = 0.0;
    msg.h_acceleration_target = 0.0;
    msg.force_target = 0.0;
    msg.l_velocity_target = 0.0;
    msg.l_acceleration_target = 0.0;
    return msg;
  }

  void processHighLevelCommands() {
    switch (cmd_.command) {
      case boro_interfaces::msg::TaskManagerCommand::CMD_INIT:
        if (state_ == TaskState::ERROR_HOLD) {
          error_code_ = boro_interfaces::msg::BowEvent::NONE;
          error_text_.clear();
        }
        transitionTo(TaskState::STARTUP, "CMD_INIT");
        cmd_.command = boro_interfaces::msg::TaskManagerCommand::CMD_NONE;
        break;

      case boro_interfaces::msg::TaskManagerCommand::CMD_HOME:
        transitionTo(TaskState::HOME, "CMD_HOME");
        cmd_.command = boro_interfaces::msg::TaskManagerCommand::CMD_NONE;
        break;

      case boro_interfaces::msg::TaskManagerCommand::CMD_SELECT_CONFIG:
        if (!config_selected_event_sent_) {
          publishEvent(boro_interfaces::msg::BowEvent::CONFIG_SELECTED,
                       "configuration selected: string=" + cmd_.selected_string, false);
          config_selected_event_sent_ = true;
        }
        transitionTo(TaskState::CONFIG_SELECTED, "CMD_SELECT_CONFIG");
        cmd_.command = boro_interfaces::msg::TaskManagerCommand::CMD_NONE;
        break;

      case boro_interfaces::msg::TaskManagerCommand::CMD_ROTATE:
        transitionTo(TaskState::ROTATING, "CMD_ROTATE");
        cmd_.command = boro_interfaces::msg::TaskManagerCommand::CMD_NONE;
        break;

      case boro_interfaces::msg::TaskManagerCommand::CMD_LOWER_BOW:
        transitionTo(TaskState::LOWER_BOW, "CMD_LOWER_BOW");
        cmd_.command = boro_interfaces::msg::TaskManagerCommand::CMD_NONE;
        break;

      case boro_interfaces::msg::TaskManagerCommand::CMD_BOWING:
        if (state_ == TaskState::CONTACT_SETTLE || state_ == TaskState::HOLD_FORCE ||
            state_ == TaskState::LOWER_BOW || state_ == TaskState::ROTATING ||
            state_ == TaskState::CONFIG_SELECTED) {
          transitionTo(TaskState::BOWING, "CMD_BOWING");
        }
        cmd_.command = boro_interfaces::msg::TaskManagerCommand::CMD_NONE;
        break;

      case boro_interfaces::msg::TaskManagerCommand::CMD_RAISE_BOW:
        transitionTo(TaskState::RAISE_BOW, "CMD_RAISE_BOW");
        cmd_.command = boro_interfaces::msg::TaskManagerCommand::CMD_NONE;
        break;

      case boro_interfaces::msg::TaskManagerCommand::CMD_FINISH:
        if (!shutdown_selected_event_sent_) {
          publishEvent(boro_interfaces::msg::BowEvent::SHUTDOWN_SELECTED, "shutdown selected",
                       false);
          shutdown_selected_event_sent_ = true;
        }
        transitionTo(TaskState::FINISH, "CMD_FINISH");
        cmd_.command = boro_interfaces::msg::TaskManagerCommand::CMD_NONE;
        break;

      case boro_interfaces::msg::TaskManagerCommand::CMD_ERROR:
        fail(boro_interfaces::msg::BowEvent::ERROR, "commanded error state");
        cmd_.command = boro_interfaces::msg::TaskManagerCommand::CMD_NONE;
        break;

      case boro_interfaces::msg::TaskManagerCommand::CMD_NONE:
      default:
        break;
    }
  }

  bool updateContactReachedHysteresis(double dt) {
    const bool above_on = latest_state_.in_contact || (latest_state_.force >= force_contact_on_);
    if (above_on) {
      contact_reached_accum_sec_ += dt;
    } else {
      contact_reached_accum_sec_ = 0.0;
    }
    return contact_reached_accum_sec_ >= contact_reached_hold_sec_;
  }

  bool updateContactLostHysteresis(double dt) {
    const bool below_off =
        (!latest_state_.in_contact) && (latest_state_.force <= force_contact_off_);
    if (below_off) {
      contact_lost_accum_sec_ += dt;
    } else {
      contact_lost_accum_sec_ = 0.0;
    }
    return contact_lost_accum_sec_ >= contact_lost_hold_sec_;
  }

  void update() {
    if (!state_received_) {
      return;
    }

    static rclcpp::Time prev_update_time = now();
    const rclcpp::Time now_t = now();
    double dt = (now_t - prev_update_time).seconds();
    if (!std::isfinite(dt) || dt <= 0.0 || dt > 0.1) {
      dt = 1.0 / std::max(loop_hz_, 1.0);
    }
    prev_update_time = now_t;

    // if (latest_state_.force > force_max_) {
    //   fail(boro_interfaces::msg::BowEvent::ERROR, "normal force above limit");
    //   return;
    // }

    if (latest_state_.h < h_min_ || latest_state_.h > h_max_ || latest_state_.l < l_min_ ||
        latest_state_.l > l_max_) {
      fail(boro_interfaces::msg::BowEvent::ERROR, "joint limit reached");
      return;
    }

    processHighLevelCommands();

    const double elapsed = (now_t - state_enter_time_).seconds();
    auto target = makeBaseTarget();

    switch (state_) {
      case TaskState::STARTUP: {
        target.alpha_target = home_alpha_;
        target.h_target = home_h_;
        target.l_target = home_l_;

        if (elapsed >= startup_wait_sec_) {
          if (!ready_sent_) {
            publishEvent(boro_interfaces::msg::BowEvent::READY, "task manager ready", false);
            ready_sent_ = true;
          }
          // transitionTo(TaskState::HOME, "startup timer elapsed");
          transitionTo(TaskState::ROTATING, "startup timer elapsed");
          // transitionTo(TaskState::TESTING, "startup timer elapsed");
        }
      } break;
      case TaskState::TESTING: {
        target.alpha_target = home_alpha_;
        target.h_mode = boro_interfaces::msg::BowTarget::MODE_POSITION;
        target.h_target = home_h_;
        target.l_mode = boro_interfaces::msg::BowTarget::MODE_VELOCITY;
        const double v = 0.1;
        target.l_velocity_target = v;

        const double l_end = (v >= 0.0) ? cmd_.bow_range_max : cmd_.bow_range_min;
        const bool reached_end =
            (v >= 0.0 && latest_state_.l >= l_end) || (v < 0.0 && latest_state_.l <= l_end);

        if (reached_end) {
          if (!config_played_event_sent_) {
            publishEvent(boro_interfaces::msg::BowEvent::CONFIG_PLAYED,
                         "configured stroke completed", false);
            config_played_event_sent_ = true;
          }
          if (cmd_.auto_continue || all_auto_continue_) {
            transitionTo(TaskState::RAISE_BOW, "stroke finished");
          }
        }

      } break;

      case TaskState::HOME: {
        target.alpha_target = home_alpha_;
        target.h_target = home_h_;
        target.l_target = home_l_;

        if (homeReached() && !home_event_sent_) {
          publishEvent(boro_interfaces::msg::BowEvent::HOME_REACHED, "home reached", false);
          home_event_sent_ = true;
          if (cmd_.auto_continue || all_auto_continue_) {
            transitionTo(TaskState::ROTATING, "auto continue from home");
          }
        }

        if (elapsed > home_timeout_sec_ && !homeReached()) {
          fail(boro_interfaces::msg::BowEvent::TIMEOUT, "home timeout");
          return;
        }
      } break;

      case TaskState::CONFIG_SELECTED: {
        target.alpha_target = selectedAlpha();
        target.h_target = safe_h_;
        target.l_target = start_l_;

        if (cmd_.auto_continue || all_auto_continue_) {
          transitionTo(TaskState::ROTATING, "auto continue after config selection");
        }
      } break;

      case TaskState::ROTATING: {
        target.alpha_target = selectedAlpha();
        target.h_target = safe_h_;
        target.l_target = start_l_;

        const bool angle_ok = near(latest_state_.alpha, target.alpha_target, angle_tol_);
        const bool h_ok = near(latest_state_.h, safe_h_, home_tol_h_);
        const bool l_ok = near(latest_state_.l, start_l_, pose_tol_l_);

        if (angle_ok && h_ok && l_ok) {
          if (!angle_reached_event_sent_) {
            publishEvent(boro_interfaces::msg::BowEvent::ANGLE_REACHED,
                         "target angle and pre-contact pose reached", false);
            angle_reached_event_sent_ = true;
          }
          if (cmd_.auto_continue || all_auto_continue_) {
            transitionTo(TaskState::LOWER_BOW, "auto continue after rotate");
          }
        }
      } break;

      case TaskState::LOWER_BOW: {
        target.alpha_target = selectedAlpha();
        target.l_target = start_l_;
        target.h_mode = boro_interfaces::msg::BowTarget::MODE_VELOCITY;
        target.h_velocity_target = cmd_.h_approach_velocity;

        // const bool contact_reached = updateContactReachedHysteresis(dt);
        const bool contact_reached = latest_state_.force >= force_contact_on_;

        if (contact_reached) {
          contact_h_ = latest_state_.h + force_h_offset_;

          if (!contact_event_sent_) {
            publishEvent(boro_interfaces::msg::BowEvent::CONTACT_REACHED,
                         "contact reached, memorized contact h", false);
            contact_event_sent_ = true;
          }

          // transitionTo(TaskState::CONTACT_SETTLE, "contact detected with hysteresis");
          transitionTo(TaskState::HOLD_FORCE, "contact detected with hysteresis");

        } else if (elapsed > lower_timeout_sec_) {
          fail(boro_interfaces::msg::BowEvent::NO_CONTACT_DETECTED, "contact not detected in time");
          return;
        }
      } break;

      case TaskState::CONTACT_SETTLE: {
        target.alpha_target = selectedAlpha();
        target.l_target = start_l_;
        target.h_mode = boro_interfaces::msg::BowTarget::MODE_POSITION;
        target.h_target = contact_h_;
        target.h_velocity_target = 0.0;
        target.h_acceleration_target = 0.0;

        // const bool contact_lost = updateContactLostHysteresis(dt);
        // if (contact_lost) {
        //   fail(boro_interfaces::msg::BowEvent::CONTACT_LOST, "contact lost during contact
        //   settle"); return;
        // }

        const bool h_stable = near(latest_state_.h, contact_h_, contact_h_tolerance_);
        // const bool force_present = latest_state_.force >= contact_ready_force_min_;

        if (elapsed >= contact_settle_time_sec_ && h_stable) {
          if (!contact_ready_event_sent_) {
            publishEvent(boro_interfaces::msg::BowEvent::CONTACT_READY, "contact stabilized, ready",
                         false);
            contact_ready_event_sent_ = true;
          }

          if (cmd_.auto_continue || all_auto_continue_) {
            transitionTo(TaskState::HOLD_FORCE, "contact stabilized");
          }
        }
      } break;

      case TaskState::HOLD_FORCE: {
        target.alpha_target = selectedAlpha();
        target.l_mode = boro_interfaces::msg::BowTarget::MODE_VELOCITY;
        target.l_target = 0;
        target.h_mode = boro_interfaces::msg::BowTarget::MODE_FORCE;
        target.force_target = cmd_.desired_force;

        // const bool contact_lost = updateContactLostHysteresis(dt);
        // if (contact_lost) {
        //   fail(boro_interfaces::msg::BowEvent::CONTACT_LOST, "contact lost before bowing");
        //   return;
        // }

        if (cmd_.auto_continue || all_auto_continue_) {
          const bool force_ok = std::abs(latest_state_.force - cmd_.desired_force) <= force_tol_;
          if (force_ok) {
            transitionTo(TaskState::BOWING, "auto continue after force hold");
          }
        }
      } break;

      case TaskState::BOWING: {
        target.alpha_target = selectedAlpha();
        target.h_mode = boro_interfaces::msg::BowTarget::MODE_FORCE;
        target.force_target = cmd_.desired_force;
        target.l_mode = boro_interfaces::msg::BowTarget::MODE_VELOCITY;
        // target.l_velocity_target = cmd_.bow_velocity;
        const double v = -0.1;
        target.l_velocity_target = v;

        // const bool contact_lost = updateContactLostHysteresis(dt);
        // if (contact_lost) {
        //   fail(boro_interfaces::msg::BowEvent::CONTACT_LOST, "contact lost during bowing");
        //   return;
        // }

        const double l_end = (v >= 0.0) ? cmd_.bow_range_max : cmd_.bow_range_min;
        const bool reached_end =
            (v >= 0.0 && latest_state_.l >= l_end) || (v < 0.0 && latest_state_.l <= l_end);

        if (reached_end) {
          if (!config_played_event_sent_) {
            publishEvent(boro_interfaces::msg::BowEvent::CONFIG_PLAYED,
                         "configured stroke completed", false);
            config_played_event_sent_ = true;
          }
          if (cmd_.auto_continue || all_auto_continue_) {
            transitionTo(TaskState::RAISE_BOW, "stroke finished");
          }
        }
      } break;

      case TaskState::RAISE_BOW: {
        target.alpha_target = selectedAlpha();
        target.l_target = latest_state_.l;
        target.h_mode = boro_interfaces::msg::BowTarget::MODE_VELOCITY;
        target.h_velocity_target = cmd_.h_raise_velocity;

        if (latest_state_.h >= safe_h_) {
          if (!bow_raised_event_sent_) {
            publishEvent(boro_interfaces::msg::BowEvent::BOW_RAISED, "bow raised to safe height",
                         false);
            bow_raised_event_sent_ = true;
          }
          if (cmd_.auto_continue || all_auto_continue_) {
            transitionTo(TaskState::RETURN_HOME, "safe height reached");
          }
        } else if (elapsed > raise_timeout_sec_) {
          fail(boro_interfaces::msg::BowEvent::TIMEOUT, "raise timeout");
          return;
        }
      } break;

      case TaskState::RETURN_HOME: {
        target.alpha_target = home_alpha_;
        target.h_target = home_h_;
        target.l_target = home_l_;

        if (homeReached()) {
          transitionTo(TaskState::HOME, "returned home");
        } else if (elapsed > home_timeout_sec_) {
          fail(boro_interfaces::msg::BowEvent::TIMEOUT, "return home timeout");
          return;
        }
      } break;

      case TaskState::FINISH: {
        target.alpha_target = home_alpha_;
        target.h_target = home_h_;
        target.l_target = home_l_;
        target.enable = false;
      } break;

      case TaskState::ERROR_HOLD: {
        target.alpha_target = latest_state_.alpha;
        target.h_target = clamp(latest_state_.h + 0.002, h_min_, h_max_);
        target.l_target = latest_state_.l;
        target.enable = true;
      } break;
    }

    target_pub_->publish(target);
  }
};

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<BoroTaskManagerNode>());
  rclcpp::shutdown();
  return 0;
}