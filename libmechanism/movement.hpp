#ifndef LIB_MECHANISM_MOVEMENT_HPP_
#define LIB_MECHANISM_MOVEMENT_HPP_

/** @file multi_stepper.hpp
 *  @brief Multi stepper device class definition
 *
 * Multi stepper devices using GPIO
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
enum class unit {
  cm,
  mm,
};
}  // namespace movement

using MovementBuilder = StaticObj<impl::MovementBuilderImpl>;

namespace impl {
class MovementBuilderImpl : public StackObj {
  friend class Movement;

 private:
  template <class MovementBuilderImpl>
  template <typename... Args>
  friend ATM_STATUS StaticObj<MovementBuilderImpl>::create(Args&&... args);

  /**
   * Setup movement devices for X-Axis
   *
   * @param stepper_x_id      instance id of stepper x in StepperDeviceRegistry
   * @param limit_switch_x_id instance id of limit switch x in
   * DigitalInputRegistry
   *
   * @return ATM_STATUS ATM_OK or ATM_ERR, but not both
   */
  ATM_STATUS setup_x(const char* stepper_x_id, const char* limit_switch_x_id);
  /**
   * Setup movement devices for Y-Axis
   *
   * @param stepper_y_id      instance id of stepper y in StepperDeviceRegistry
   * @param limit_switch_y_id instance id of limit switch y in
   * DigitalInputRegistry
   *
   * @return ATM_STATUS ATM_OK or ATM_ERR, but not both
   */
  ATM_STATUS setup_y(const char* stepper_y_id, const char* limit_switch_y_id);
  /**
   * Setup movement devices for Z-Axis
   *
   * @param stepper_z_id      instance id of stepper z in StepperDeviceRegistry
   * @param limit_switch_z_id instance id of limit switch z in
   * DigitalInputRegistry
   *
   * @return ATM_STATUS ATM_OK or ATM_ERR, but not both
   */
  ATM_STATUS setup_z(const char* stepper_z_id, const char* limit_switch_z_id);

  /**
   * Build movement mechanism
   *
   * @return shared_ptr of Movement
   */
  std::shared_ptr<Movement> build();
  /**
   * Get Steps per mm
   *
   * @return steps_per_mm
   */
  const device::stepper::step& steps_per_mm() const { return steps_per_mm_; }
  /**
   * Get Stepper X instance ID
   *
   * @return instance id of stepper x
   */
  const std::string& stepper_x_id() const { return stepper_x_id_; }
  /**
   * Get Stepper Y instance ID
   *
   * @return instance id of stepper y
   */
  const std::string& stepper_y_id() const { return stepper_y_id_; }
  /**
   * Get Stepper Z instance ID
   *
   * @return instance id of stepper z
   */
  const std::string& stepper_z_id() const { return stepper_z_id_; }
  /**
   * Get limit switch X instance ID
   *
   * @return instance id of stepper x
   */
  const std::string& limit_switch_x_id() const { return limit_switch_x_id_; }
  /**
   * Get limit switch Y instance ID
   *
   * @return instance id of limit_switch y
   */
  const std::string& limit_switch_y_id() const { return limit_switch_y_id_; }
  /**
   * Get limit switch Z instance ID
   *
   * @return instance id of limit switch z
   */
  const std::string& limit_switch_z_id() const { return limit_switch_z_id_; }

 private:
  /**
   * MovementBuilderImpl Constructor
   *
   * Builder pattern for movement mechanism
   */
  explicit MovementBuilderImpl(const device::stepper::step& steps_per_mm);
  /**
   * MovementBuilderImpl Destructor
   *
   * Will destroy all the spdlog logger instances
   */
  ~MovementBuilderImpl() = default;

 private:
  /**
   * Conversion of mm to steps
   *
   * 1 mm = x steps
   */
  const device::stepper::step steps_per_mm_;
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
   * Instance id of limit switch z
   */
  std::string limit_switch_z_id_;
};
}  // namespace impl

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
   * Move only stepper x
   */
  template <movement::unit Unit>
  void move_x(double length);
  /**
   * Move only stepper y
   */
  template <movement::unit Unit>
  void move_y(double length);
  /**
   * Move only stepper z
   */
  template <movement::unit Unit>
  void move_z(double length);
  /**
   * Stop only stepper x
   *
   * @return remaining steps of stepper x
   */
  const device::stepper::step stop_x(void);
  /**
   * Stop only stepper y
   *
   * @return remaining steps of stepper y
   */
  const device::stepper::step stop_y(void);
  /**
   * Stop only z stepper
   *
   * @return remaining steps of stepper z
   */
  const device::stepper::step stop_z(void);
  /**
   * Stop all steppers
   */
  void stop(void);
  /**
   * Check if movement mechanism is active or not
   *
   * @return active (true/false)
   */
  const bool active() const { return active_; };
  /**
   * Homing all stepper
   *
   * @return homing completed
   */
  bool homing();

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
  virtual ~Movement() = default;
  /**
   * Convert steps_per_mm to specified length
   *
   * @return steps
   */
  template <movement::unit Unit>
  static long convert_length_to_steps(
      double                       length,
      const device::stepper::step& steps_per_mm);
  /**
   * Get thread pool instance
   *
   * @return instance of algo::ThreadPool
   */
  algo::ThreadPool& thread_pool() { return thread_pool_; }
  /**
   * Get Steps per mm
   *
   * @return steps_per_mm
   */
  const device::stepper::step& steps_per_mm() const { return steps_per_mm_; }
  /**
   * Get Stepper X instance of StepperDevice that has been initialized
   *
   * @return shared_ptr of StepperDevice
   */
  const std::shared_ptr<device::StepperDevice>& stepper_x() const {
    return stepper_x_;
  }
  /**
   * Get Stepper Y instance of StepperDevice that has been initialized
   *
   * @return shared_ptr of StepperDevice
   */
  const std::shared_ptr<device::StepperDevice>& stepper_y() const {
    return stepper_y_;
  }
  /**
   * Get Stepper Z instance of StepperDevice that has been initialized
   *
   * @return shared_ptr of StepperDevice
   */
  const std::shared_ptr<device::StepperDevice>& stepper_z() const {
    return stepper_z_;
  }
  /**
   * Get Limit Switch X instance of DigitalInputDevice that has been initialized
   *
   * @return shared_ptr of DigitalInputDevice
   */
  const std::shared_ptr<device::DigitalInputDevice>& limit_switch_x() const {
    return limit_switch_x_;
  }
  /**
   * Get Limit Switch Y instance of DigitalInputDevice that has been initialized
   *
   * @return shared_ptr of DigitalInputDevice
   */
  const std::shared_ptr<device::DigitalInputDevice>& limit_switch_y() const {
    return limit_switch_y_;
  }
  /**
   * Get Limit Switch Z instance of DigitalInputDevice that has been initialized
   *
   * @return shared_ptr of DigitalInputDevice
   */
  const std::shared_ptr<device::DigitalInputDevice>& limit_switch_z() const {
    return limit_switch_z_;
  }
  /**
   * Setup all stepper devices
   *
   * Will return early if fails
   */
  void setup_stepper(const impl::MovementBuilderImpl* builder);
  /**
   * Setup all limit switch devices
   *
   * Will return early if fails
   */
  void setup_limit_switch(const impl::MovementBuilderImpl* builder);

 private:
  algo::ThreadPool thread_pool_;
  /**
   * Conversion of mm to steps
   *
   * 1 mm = x steps
   */
  const device::stepper::step steps_per_mm_;
  /**
   * Check wether movement mechanism is usable or not
   */
  bool active_;
  /**
   * Get Stepper X-Axis that has been initialized
   *
   * @return shared_ptr of StepperDevice
   */
  std::shared_ptr<device::StepperDevice> stepper_x_;
  /**
   * Get Stepper Y-Axis that has been initialized
   *
   * @return shared_ptr of StepperDevice
   */
  std::shared_ptr<device::StepperDevice> stepper_y_;
  /**
   * Get Stepper Z-Axis that has been initialized
   *
   * @return shared_ptr of StepperDevice
   */
  std::shared_ptr<device::StepperDevice> stepper_z_;
  /**
   * Get Limit Switch for X-Axis that has been initialized
   *
   * @return shared_ptr of DigitalInputDevice
   */
  std::shared_ptr<device::DigitalInputDevice> limit_switch_x_;
  /**
   * Get Limit Switch for Y-Axis that has been initialized
   *
   * @return shared_ptr of DigitalInputDevice
   */
  std::shared_ptr<device::DigitalInputDevice> limit_switch_y_;
  /**
   * Get Limit Switch for Z-Axis that has been initialized
   *
   * @return shared_ptr of DigitalInputDevice
   */
  std::shared_ptr<device::DigitalInputDevice> limit_switch_z_;
};
}  // namespace mechanism

NAMESPACE_END

#endif  // LIB_MECHANISM_MOVEMENT_HPP_
