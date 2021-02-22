#ifndef LIB_MECHANISM_MOVEMENT_HPP_
#define LIB_MECHANISM_MOVEMENT_HPP_

/** @file movement.hpp
 *  @brief Movement mechanism class definition
 *
 * Movement mechanism
 */

#include <memory>
#include <string>

#include <libcore/core.hpp>
#include <libdevice/device.hpp>

NAMESPACE_BEGIN

namespace mechanism {
// forward declaration
namespace movement {
enum class unit;
}
namespace impl {
class MovementBuilderImpl;
}
class Movement;

namespace movement {
enum class unit { cm, mm };
}  // namespace movement

using MovementBuilder = StaticObj<impl::MovementBuilderImpl>;

namespace impl {
/**
 * @brief MovementBuilderImpl implementation.
 *        This is a class that implement builder pattern for Movement class
 *
 * Builder for movement class to reduce verbosity
 *
 * This class can be singleton because it should be only one movement mechanism
 * that will be implemented in single app
 *
 * @author Ray Andrew
 * @date   May 2020
 */
class MovementBuilderImpl : public StackObj {
  friend class Movement;

  template <class MovementBuilderImpl>
  template <typename... Args>
  friend ATM_STATUS StaticObj<MovementBuilderImpl>::create(Args&&... args);

 public:
  /**
   * Setup movement devices for X-Axis
   *
   * @param stepper_x_id      instance id of stepper x in StepperDeviceRegistry
   * @param steps_per_mm      steps conversion to mm
   * @param limit_switch_x_id instance id of limit switch x in
   * DigitalInputRegistry
   *
   * @return ATM_STATUS ATM_OK or ATM_ERR, but not both
   */
  ATM_STATUS setup_x(const std::string&           stepper_x_id,
                     const device::stepper::step& steps_per_mm,
                     const std::string&           limit_switch_x_id);
  /**
   * Setup movement devices for Y-Axis
   *
   * @param stepper_y_id      instance id of stepper y in StepperDeviceRegistry
   * @param steps_per_mm      steps conversion to mm
   * @param limit_switch_y_id instance id of limit switch y in
   * DigitalInputRegistry
   *
   * @return ATM_STATUS ATM_OK or ATM_ERR, but not both
   */
  ATM_STATUS setup_y(const std::string&           stepper_y_id,
                     const device::stepper::step& steps_per_mm,
                     const std::string&           limit_switch_y_id);
  /**
   * Setup movement devices for Z-Axis
   *
   * @param stepper_z_id             instance id of stepper z in
   * StepperDeviceRegistry
   * @param steps_per_mm             steps conversion to mm
   * @param limit_switch_z_top_id    instance id of limit switch z (upper bound)
   * in DigitalInputRegistry
   * @param limit_switch_z_bottom_id instance id of limit switch z (lower bound)
   * in DigitalInputRegistry
   *
   * @return ATM_STATUS ATM_OK or ATM_ERR, but not both
   */
  ATM_STATUS setup_z(const std::string&           stepper_z_id,
                     const device::stepper::step& steps_per_mm,
                     const std::string&           limit_switch_z_top_id,
                     const std::string&           limit_switch_z_bottom_id);
  /**
   * Setup finger
   *
   * @param finger_id            finger instance id
   * @param finger_brake_id      finger instance id
   * @param finger_infrared_id   finger infrared id
   *
   * @return ATM_STATUS ATM_OK or ATM_ERR, but not both
   */
  ATM_STATUS setup_finger(const std::string& finger_id,
                          const std::string& finger_brake_id,
                          const std::string& finger_infrared_id);
  /**
   * Build movement mechanism
   *
   * @return shared_ptr of Movement
   */
  std::shared_ptr<Movement>& build();
  /**
   * Get Steps per mm of x-axis
   *
   * @return steps_per_mm
   */
  inline const device::stepper::step& steps_per_mm_x() const {
    return steps_per_mm_x_;
  }
  /**
   * Get Steps per mm of y-axis
   *
   * @return steps_per_mm
   */
  inline const device::stepper::step& steps_per_mm_y() const {
    return steps_per_mm_y_;
  }
  /**
   * Get Steps per mm of z-axis
   *
   * @return steps_per_mm
   */
  inline const device::stepper::step& steps_per_mm_z() const {
    return steps_per_mm_z_;
  }
  /**
   * Get Stepper X instance ID
   *
   * @return instance id of stepper x
   */
  inline const std::string& stepper_x_id() const { return stepper_x_id_; }
  /**
   * Get Stepper Y instance ID
   *
   * @return instance id of stepper y
   */
  inline const std::string& stepper_y_id() const { return stepper_y_id_; }
  /**
   * Get Stepper Z instance ID
   *
   * @return instance id of stepper z
   */
  inline const std::string& stepper_z_id() const { return stepper_z_id_; }
  /**
   * Get limit switch X instance ID
   *
   * @return instance id of stepper x
   */
  inline const std::string& limit_switch_x_id() const {
    return limit_switch_x_id_;
  }
  /**
   * Get limit switch Y instance ID
   *
   * @return instance id of limit_switch y
   */
  inline const std::string& limit_switch_y_id() const {
    return limit_switch_y_id_;
  }
  /**
   * Get limit switch Z (upper bound) instance ID
   *
   * @return instance id of limit switch z (upper bound)
   */
  inline const std::string& limit_switch_z_top_id() const {
    return limit_switch_z_top_id_;
  }
  /**
   * Get limit switch Z (lower bound) instance ID
   *
   * @return instance id of limit switch z (lower bound)
   */
  inline const std::string& limit_switch_z_bottom_id() const {
    return limit_switch_z_bottom_id_;
  }
  /**
   * Get finger motor instance ID
   *
   * @return instance id of finger
   */
  inline const std::string& finger_id() const { return finger_id_; }
  /**
   * Get finger brake instance ID
   *
   * @return instance id of finger brake
   */
  inline const std::string& finger_brake_id() const { return finger_brake_id_; }
  /**
   * Get finger infrared instance ID
   *
   * @return instance id of finger infrared
   */
  inline const std::string& finger_infrared_id() const {
    return finger_infrared_id_;
  }
  /**
   * Get movement mechanism instance
   *
   * @return instance of movement mechanism
   */
  inline std::shared_ptr<Movement> movement() {
    return movement_mechanism_instance_;
  }

 private:
  /**
   * MovementBuilderImpl Constructor
   *
   * Builder pattern for movement mechanism
   */
  explicit MovementBuilderImpl();
  /**
   * MovementBuilderImpl Destructor
   *
   * Will destroy all the spdlog logger instances
   */
  ~MovementBuilderImpl();

 private:
  /**
   * Movement mechanism instance
   */
  std::shared_ptr<Movement> movement_mechanism_instance_;
  /**
   * Conversion of mm to steps for x-axis
   *
   * 1 mm = x steps
   */
  device::stepper::step steps_per_mm_x_;
  /**
   * Conversion of mm to steps for y-axis
   *
   * 1 mm = x steps
   */
  device::stepper::step steps_per_mm_y_;
  /**
   * Conversion of mm to steps for z-axis
   *
   * 1 mm = x steps
   */
  device::stepper::step steps_per_mm_z_;
  /**
   * Instance id of stepper x
   */
  std::string stepper_x_id_;
  /**
   * Instance id of stepper y
   */
  std::string stepper_y_id_;
  /**
   * Instance id of stepper z
   */
  std::string stepper_z_id_;
  /**
   * Instance id of limit switch x
   */
  std::string limit_switch_x_id_;
  /**
   * Instance id of limit switch y
   */
  std::string limit_switch_y_id_;
  /**
   * Instance id of limit switch z (upper bound)
   */
  std::string limit_switch_z_top_id_;
  /**
   * Instance id of limit switch z (lower bound)
   */
  std::string limit_switch_z_bottom_id_;
  /**
   * Instance id of finger
   */
  std::string finger_id_;
  /**
   * Instance id of finger brake
   */
  std::string finger_brake_id_;
  /**
   * Instance id of finger infrared
   */
  std::string finger_infrared_id_;
};
}  // namespace impl

/** Getter for movement_mechanism */
auto movement_mechanism = []() {
  massert(MovementBuilder::get() != nullptr, "sanity");
  return MovementBuilder::get()->movement();
};

class Movement : public StackObj {
 public:
  /**
   * Create shared_ptr<Movement>
   *
   * Pass every args to Movement()
   *
   * @param args arguments that will be passed to Movement()
   */
  MAKE_STD_SHARED(Movement)

 public:
  /**
   * Move single or multi steppers at once
   *
   * @param x  length of x-axis
   * @param y  length of y-axis
   * @param z  length of z-axis
   */
  template <movement::unit Unit>
  void move(Point x, Point y, Point z);
  /**
   * Movement progress in percentage
   *
   * @return percentage of progress
   */
  float progress() const;
  /**
   * Stop only stepper x
   *
   * @return remaining steps of stepper x
   */
  device::stepper::step stop_x(void);
  /**
   * Stop only stepper y
   *
   * @return remaining steps of stepper y
   */
  device::stepper::step stop_y(void);
  /**
   * Stop only z stepper
   *
   * @return remaining steps of stepper z
   */
  device::stepper::step stop_z(void);
  /**
   * Stop all steppers
   */
  void stop(void);
  /**
   * Check if movement mechanism is active or not
   *
   * This is for checking the initialization
   *
   * @return active (true/false)
   */
  inline bool active() const { return active_; }
  /**
   * Get status of ready to start a new move or not
   *
   * @return ready or not
   */
  inline const bool& ready() const { return ready_; }
  /**
   * Homing all stepper
   */
  void homing();
  /**
   * Homing finger
   */
  void homing_finger() const;
  /**
   * Check is home or not
   *
   * @return is home or not
   */
  bool is_home() const;
  /**
   * Rotate finger
   */
  void rotate_finger() const;
  /**
   * Stop finger
   */
  void stop_finger() const;
  /**
   * Move finger down
   */
  void move_finger_down();
  /**
   * Move finger up
   */
  void move_finger_up();
  /**
   * Move to position zero spraying
   */
  void move_to_spraying_position();
  /**
   * Move to position zero tending
   */
  void move_to_tending_position();
  /**
   * Move according to spraying paths
   */
  void follow_spraying_paths();
  /**
   * Move according to edge tending paths
   */
  void follow_tending_paths_edge();
  /**
   * Move according to zigzag tending paths
   */
  void follow_tending_paths_zigzag();
  /**
   * Enable all motors
   */
  void enable_motors() const;
  /**
   * Disable all motors
   */
  void disable_motors() const;
  /**
   * Set Motor Profile for steppers
   *
   * @param speed_profile speed profile configuration
   **/
  void motor_profile(const config::MechanismSpeed& speed_profile) const;

 private:
  /**
   * Movement Constructor
   *
   * Initialize the movement mechanism by getting the stepper devices
   *
   * @param  builder  movement mechanism builder
   */
  Movement(const impl::MovementBuilderImpl* builder);
  /**
   * Movement Destructor
   *
   * Clean up movement mechanism
   */
  ~Movement();
  /**
   * Convert steps_per_mm to specified length
   *
   * @param length       length to be converted to steps
   * @param steps_per_mm steps per mm configuration
   *
   * @return steps
   */
  template <movement::unit Unit>
  static long convert_length_to_steps(
      double                       length,
      const device::stepper::step& steps_per_mm);
  /**
   * Get instance of impl::MovementBuilderImpl
   *
   * @return pointer of impl::MovementBuilderImpl
   */
  inline const impl::MovementBuilderImpl* builder() const { return builder_; }
  /**
   * Get Stepper X instance of StepperDevice that has been initialized
   *
   * @return shared_ptr of StepperDevice
   */
  inline const std::shared_ptr<device::StepperDevice>& stepper_x() const {
    return stepper_x_;
  }
  /**
   * Get Stepper Y instance of StepperDevice that has been initialized
   *
   * @return shared_ptr of StepperDevice
   */
  inline const std::shared_ptr<device::StepperDevice>& stepper_y() const {
    return stepper_y_;
  }
  /**
   * Get Stepper Z instance of StepperDevice that has been initialized
   *
   * @return shared_ptr of StepperDevice
   */
  inline const std::shared_ptr<device::StepperDevice>& stepper_z() const {
    return stepper_z_;
  }
  /**
   * Get Limit Switch X instance of DigitalInputDevice that has been initialized
   *
   * @return shared_ptr of DigitalInputDevice
   */
  inline const std::shared_ptr<device::DigitalInputDevice>& limit_switch_x()
      const {
    return limit_switch_x_;
  }
  /**
   * Get Limit Switch Y instance of DigitalInputDevice that has been initialized
   *
   * @return shared_ptr of DigitalInputDevice
   */
  inline const std::shared_ptr<device::DigitalInputDevice>& limit_switch_y()
      const {
    return limit_switch_y_;
  }
  /**
   * Get Limit Switch Z (upper bound) instance of DigitalInputDevice that has
   * been initialized
   *
   * @return shared_ptr of DigitalInputDevice
   */
  inline const std::shared_ptr<device::DigitalInputDevice>& limit_switch_z_top()
      const {
    return limit_switch_z_top_;
  }
  /**
   * Get Limit Switch Z (lower bound) instance of DigitalInputDevice that has
   * been initialized
   *
   * @return shared_ptr of DigitalInputDevice
   */
  inline const std::shared_ptr<device::DigitalInputDevice>&
  limit_switch_z_bottom() const {
    return limit_switch_z_bottom_;
  }
  /**
   * Get Finger instance of PWMDevice that has
   * been initialized
   *
   * @return shared_ptr of PWMDevice
   */
  inline const std::shared_ptr<device::PWMDevice>& finger() const {
    return finger_;
  }
  /**
   * Get Finger brake instance of DigitalOutputDevice that has
   * been initialized
   *
   * @return shared_ptr of DigitalOutputDevice
   */
  inline const std::shared_ptr<device::DigitalOutputDevice>& finger_brake()
      const {
    return finger_brake_;
  }
  /**
   * Get Finger infrared instance of DigitalInputDevice that has
   * been initialized
   *
   * @return shared_ptr of DigitalInputDevice
   */
  inline const std::shared_ptr<device::DigitalInputDevice>& finger_infrared()
      const {
    return finger_infrared_;
  }
  /**
   * Setup all stepper devices
   *
   * Will return early if fails
   */
  void setup_stepper();
  /**
   * Setup all limit switch devices
   *
   * Will return early if fails
   */
  void setup_limit_switch();
  /**
   * Setup finger
   *
   * Will return early if fails
   */
  void setup_finger();
  /**
   * Setup move action for steppers
   *
   * @param x  length of x-axis
   * @param y  length of y-axis
   * @param z  length of z-axis
   */
  void start_move(const long& x, const long& y, const long& z);
  /**
   * Yield move for each step
   *
   * Will generate output to the stepper pins
   *
   * @return time until next change is needed
   */
  time_unit next();
  /**
   * Get event timers of x-axis stepper
   *
   * @return timer of x-axis stepper
   */
  inline const time_unit& event_timer_x() const { return event_timer_x_; }
  /**
   * Get event timers of y-axis stepper
   *
   * @return timer of y-axis stepper
   */
  inline const time_unit& event_timer_y() const { return event_timer_y_; }
  /**
   * Get event timers of z-axis stepper
   *
   * @return timer of z-axis stepper
   */
  inline const time_unit& event_timer_z() const { return event_timer_z_; }
  /**
   * Get timestamp of ending of last move
   *
   * @return time of last move end
   */
  inline const time_unit& last_move_end() const { return last_move_end_; }
  /**
   * Get timestamp of ending of last action
   *
   * @return next move interval time
   */
  inline const time_unit& next_move_interval() const {
    return next_move_interval_;
  }
  /**
   * Update position of x
   */
  void update_x() const;
  /**
   * Update position of y
   */
  void update_y() const;
  /**
   * Update position of z
   */
  void update_z() const;
  /**
   * Update position
   */
  void update_position() const;
  /**
   * Reverting motor params
   *
   * Reverting to homing speed profile
   */
  void revert_motor_params() const;

 private:
  /**
   * Check whether ready to start a new move or not
   */
  bool ready_;
  /**
   * When next state change is due for x-axis stepper
   */
  time_unit event_timer_x_;
  /**
   * When next state change is due for y-axis stepper
   */
  time_unit event_timer_y_;
  /**
   * When next state change is due for z-axis stepper
   */
  time_unit event_timer_z_;
  /**
   * When next state change is due for each motor
   */
  time_unit next_move_interval_;
  /**
   * When next state change is due for each motor
   */
  time_unit last_move_end_;

 private:
  /**
   * Instance of impl::MovementBuilderImpl to reduce verbosity
   */
  const impl::MovementBuilderImpl* builder_;
  /**
   * Check wether movement mechanism is usable or not
   */
  bool active_;
  /**
   * Stepper X-Axis that has been initialized
   */
  std::shared_ptr<device::StepperDevice> stepper_x_;
  /**
   * Stepper Y-Axis that has been initialized
   */
  std::shared_ptr<device::StepperDevice> stepper_y_;
  /**
   * Stepper Z-Axis that has been initialized
   */
  std::shared_ptr<device::StepperDevice> stepper_z_;
  /**
   * Limit Switch for X-Axis that has been initialized
   */
  std::shared_ptr<device::DigitalInputDevice> limit_switch_x_;
  /**
   * Limit Switch for Y-Axis that has been initialized
   */
  std::shared_ptr<device::DigitalInputDevice> limit_switch_y_;
  /**
   * Limit Switch for Z-Axis (Upper Bound) that has been initialized
   */
  std::shared_ptr<device::DigitalInputDevice> limit_switch_z_top_;
  /**
   * Limit Switch for Z-Axis (Lower Bound) that has been initialized
   */
  std::shared_ptr<device::DigitalInputDevice> limit_switch_z_bottom_;
  /**
   * Finger motor device that has been initialized
   */
  std::shared_ptr<device::PWMDevice> finger_;
  /**
   * Finger motor device that has been initialized
   */
  std::shared_ptr<device::DigitalOutputDevice> finger_brake_;
  /**
   * Finger infrared device that has been initialized
   */
  std::shared_ptr<device::DigitalInputDevice> finger_infrared_;
};
}  // namespace mechanism

NAMESPACE_END

#endif  // LIB_MECHANISM_MOVEMENT_HPP_
