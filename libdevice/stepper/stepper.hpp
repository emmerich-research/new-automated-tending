#ifndef LIB_DEVICE_DIGITAL_STEPPER_STEPPER_HPP_
#define LIB_DEVICE_DIGITAL_STEPPER_STEPPER_HPP_

/** @file stepper.hpp
 *  @brief Stepper device class definition
 *
 * Stepper device using GPIO
 */

#include <cstdlib>
#include <memory>

#include <libalgo/algo.hpp>
#include <libcore/core.hpp>

#include "gpio.hpp"

#include "digital/digital.hpp"

NAMESPACE_BEGIN

namespace device {
// forward declaration
namespace stepper {
enum class speed;
enum class state;
enum class direction;
}  // namespace stepper

template <stepper::speed Speed>
class StepperDevice;

/** device::StepperDevice registry singleton class using algo::InstanceRegistry
 */
template <stepper::speed Speed>
using StepperRegistry = algo::InstanceRegistry<StepperDevice<Speed>>;

namespace stepper {
/** Speed mode */
enum class speed {
  constant, /**< constant speed */
  linear,   /**< linear speed using equation */
};

/** Stepper state */
enum class state { stopped, accelerating, cruising, decelerating };

/** Stepper direction */
enum class direction { forward, backward };

/**
 * @var using steps = unsigned long
 * @brief Type definition for stepper steps
 */
using step = unsigned long;

/**
 * @var using pulses = unsigned long
 * @brief Type definition for stepper pulses
 */
using pulse = time_unit;
}  // namespace stepper

/**
 * @brief StepperDevice implementation.
 *
 * Originally derived from https://github.com/laurb9/StepperDriver for Arduino
 * Credits to Laurentiu Badea (laurb9)
 * MIT License
 *
 * Modified by Ray Andrew to support Raspberry PI
 *
 * StepperDevice will uses at least two Digital Output Device
 * - step pin
 * - direction pin
 * - enable pin (optional, can be jumped through)
 *
 * @tparam Speed stepper::speed option
 *
 * @author Ray Andrew
 * @date   May 2020
 */
template <stepper::speed Speed>
class StepperDevice : public StackObj {
  template <class StepperDevice>
  template <typename... Args>
  friend ATM_STATUS
  StaticObj<algo::impl::InstanceRegistryImpl<StepperDevice>>::create(
      Args&&... args);

 public:
  /**
   * Create shared_ptr<StepperDevice>
   *
   * Pass every args to StepperDevice()
   *
   * @param args arguments that will be passed to StepperDevice()
   */
  MAKE_STD_SHARED(StepperDevice)
  /**
   * Enable stepper motor
   */
  virtual void enable();
  /**
   * Disable stepper motor
   */
  virtual void disable();

  virtual void move(long steps);

 protected:
  /**
   * StepperDevice Constructor
   *
   * Initialize the stepper device by opening GPIO pins
   *
   * @param  step_pin   gpio pin, see Raspberry GPIO pinout for details
   * @param  dir_pin    gpio pin, see Raspberry GPIO pinout for details
   * @param  enable_pin gpio pin, see Raspberry GPIO pinout for details
   * @param  steps      motor steps, usually 200 steps per revolution
   */
  StepperDevice(PI_PIN        step_pin,
                PI_PIN        dir_pin,
                PI_PIN        enable_pin,
                stepper::step steps = 200);
  /**
   * StepperDevice Destructor
   *
   * Close the StepperDevice that has been initialized
   */
  virtual ~StepperDevice() = default;
  /**
   * Get Step DigitalOutputDevice that has been initialized
   *
   * @return shared_ptr of DigitalOutputDevice
   */
  const std::shared_ptr<DigitalOutputDevice>& step_device() const {
    return step_device_;
  }
  /**
   * Get Direction DigitalOutputDevice that has been initialized
   *
   * @return shared_ptr of DigitalOutputDevice
   */
  const std::shared_ptr<DigitalOutputDevice>& dir_device() const {
    return dir_device_;
  }
  /**
   * Get Enable DigitalOutputDevice that has been initialized
   *
   * @return shared_ptr of DigitalOutputDevice
   */
  const std::shared_ptr<DigitalOutputDevice>& enable_device() const {
    return enable_device_;
  }
  /**
   * Get Step GPIO pin
   *
   * @return step GPIO pin
   */
  const PI_PIN& step_pin() const { return step_pin_; }
  /**
   * Get Direction GPIO pin
   *
   * @return direction GPIO pin
   */
  const PI_PIN& dir_pin() const { return dir_pin_; }
  /**
   * Get Enable GPIO pin
   *
   * @return enable GPIO pin
   */
  const PI_PIN& enable_pin() const { return enable_pin_; }
  /**
   * Get current state of stepper
   *
   * @return current state of stepper
   */
  const stepper::state state() const;
  /**
   * Set stepper microsteps
   *
   * @param microsteps microsteps to set
   */
  virtual void microsteps(const stepper::step& microsteps);
  /**
   * Get stepper microsteps
   *
   * @return current microsteps
   */
  const stepper::step& microsteps() const { return microsteps_; }
  /**
   * Set current rpm of stepper motor
   *
   * Preferrable: 1.0 - 200.0
   *
   * @param rpm rpm to set
   */
  virtual void rpm(double rpm);
  /**
   * Get current rpm of stepper motor
   *
   * @return current rpm
   */
  const double rpm() const { return rpm_; }
  /**
   * Set acceleration of stepper motor (steps / s^2)
   *
   * @param acceleration acceleration to set
   */
  virtual void acceleration(double acceleration);
  /**
   * Get current acceleration of stepper motor
   *
   * @return current acceleration
   */
  const double acceleration() const { return acceleration_; }
  /**
   * Set deceleration of stepper motor (steps / s^2)
   *
   * @param deceleration deceleration to set
   */
  virtual void deceleration(double deceleration);
  /**
   * Get current deceleration of stepper motor
   *
   * @return current deceleration
   */
  const double deceleration() const { return deceleration_; }
  /**
   * Set motor steps
   *
   * @param rpm rpm to set
   */
  virtual void motor_steps(const stepper::step& motor_steps);
  /**
   * Get current motor steps
   *
   * @return current motor steps
   */
  const stepper::step motor_steps() const { return motor_steps_; }
  /**
   * Get stepper direction
   */
  const stepper::direction& direction() const { return direction_; }
  /**
   * Get timestamp of ending of last move
   */
  const time_unit& last_move_end() const { return last_move_end_; }
  /**
   * Get timestamp of ending of last action
   */
  const time_unit& next_move_interval() const { return next_move_interval_; }
  /**
   * Get step pulse
   */
  const stepper::step& step_pulse() const { return step_pulse_; }
  /**
   * Get remaining steps
   */
  const stepper::step& remaining_steps() const { return remaining_steps_; }
  /**
   * Get rest steps
   */
  const stepper::step& rest_steps() const { return rest_steps_; }
  /**
   * Get step count
   */
  const stepper::step& step_count() const { return step_count_; }
  /**
   * Get steps to cruise
   */
  const stepper::step& steps_to_cruise() const { return steps_to_cruise_; }
  /**
   * Get steps to brake
   */
  const stepper::step& steps_to_brake() const { return steps_to_brake_; }
  /**
   * Get remaining steps
   */
  const stepper::pulse& cruise_step_pulse() const { return cruise_step_pulse_; }

  /* Movement mechanism */
  /**
   * calculate the step pulse in microseconds for a given rpm value.
   * 60[s/min] * 1000000[us/s] / microsteps / steps / rpm
   */
  static stepper::pulse calc_step_pulse_from_rpm(
      const stepper::step& steps,
      const stepper::step& microsteps,
      double               rpm);

  /**
   * calculate the step pulse for each yield move
   */
  void calc_step_pulse();
  /**
   * Pre Start move configuration
   *
   * @param steps steps to take
   */
  void pre_start_move(long steps);
  /**
   * Start move
   *
   * @param steps steps to take
   * @param time  finish time
   */
  void start_move(long steps, time_unit time = 0);
  /**
   * Yield move for each step
   *
   * Will generate output to the stepper pins
   *
   * @return still moves or not
   */
  bool yield_move(void);
  /* End of movement mechanism */
 private:
  /**
   * tWH(STEP) pulse duration, STEP high, min value (us)
   */
  static const time_unit step_high_min;
  /**
   * Step GPIO pin
   */
  const PI_PIN step_pin_;
  /**
   * Dir GPIO pin
   */
  const PI_PIN dir_pin_;
  /**
   * Enable GPIO pin
   */
  const PI_PIN enable_pin_;
  /**
   * Step digital output device
   */
  const std::shared_ptr<DigitalOutputDevice> step_device_;
  /**
   * Direction digital output device
   */
  const std::shared_ptr<DigitalOutputDevice> dir_device_;
  /**
   * Enable digital output device
   */
  const std::shared_ptr<DigitalOutputDevice> enable_device_;
  /**
   * Stepper microsteps
   */
  stepper::step microsteps_;
  /**
   * Stepper rpm
   */
  double rpm_;
  /**
   * Stepper acceleration constant
   */
  double acceleration_;
  /**
   * Stepper deceleration constant
   */
  double deceleration_;
  /**
   * Motor steps
   *
   * Usually 200 steps / revolution
   */
  stepper::step motor_steps_;

  /* Movement mechanism variables */
  /**
   * Timestamp of ending of last move
   */
  time_unit last_move_end_;
  /**
   * Next move interval
   */
  time_unit next_move_interval_;
  /**
   * Direction state
   */
  stepper::direction direction_;
  /**
   * calculation remainder to be fed into successive steps to increase accuracy
   * (Atmel DOC8017)
   */
  stepper::step rest_steps_;
  /**
   * Remaining steps
   */
  stepper::step remaining_steps_;
  /**
   * Step counter, will be resetted for each move sequence
   */
  stepper::step step_count_;
  /**
   * Steps to cruise
   */
  stepper::step steps_to_cruise_;
  /**
   * Steps to brake
   */
  stepper::step steps_to_brake_;
  /**
   * Step pulses
   */
  stepper::pulse step_pulse_;
  /**
   * Cruise step pulses
   */
  stepper::pulse cruise_step_pulse_;
  /* End of movement mechanism variables */
};
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_DIGITAL_STEPPER_STEPPER_HPP_
