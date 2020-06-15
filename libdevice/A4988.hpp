#ifndef LIB_DEVICE_DIGITAL_STEPPER_A4988_HPP_
#define LIB_DEVICE_DIGITAL_STEPPER_A4988_HPP_

/** @file A4988.hpp
 *  @brief Pololu A4988 stepper device class definition
 *
 * Pololu A4988 Stepper device using GPIO
 */

#include <libcore/core.hpp>

#include "digital.hpp"
#include "stepper.hpp"

NAMESPACE_BEGIN

namespace device {
// forward declaration
template <stepper::speed Speed>
class A4988Device;

/** Constant speed specific implementation for Pololu A4988 Device */
using ConstantSpeedA4988Device = A4988Device<stepper::speed::constant>;

/** Linear speed specific implementation for Pololu A4988 Device */
using LinearSpeedA4988Device = A4988Device<stepper::speed::linear>;

template <stepper::speed Speed>
class A4988Device : public impl::StepperDeviceImpl<Speed> {
 public:
  /**
   * Create shared_ptr<A4988Device>
   *
   * Pass every args to A4988Device()
   *
   * @param args arguments that will be passed to A4988Device()
   */
  MAKE_STD_SHARED(A4988Device)

 protected:
  /**
   * StepperDevice Constructor
   *
   * Initialize the stepper device by opening GPIO pins
   *
   * @param  step_pin   gpio pin, see Raspberry GPIO pinout and Pololu A4988
   * sheet for details
   * @param  dir_pin    gpio pin, see Raspberry GPIO pinout and Pololu A4988
   * sheet for details
   * @param  enable_pin gpio pin, see Raspberry GPIO pinout and Pololu A4988
   * sheet for details
   * @param  ms1_pin    gpio pin, see Raspberry GPIO pinout and Pololu A4988
   * sheet for details
   * @param  ms2_pin    gpio pin, see Raspberry GPIO pinout and Pololu A4988
   * sheet for details
   * @param  ms3_pin    gpio pin, see Raspberry GPIO pinout and Pololu A4988
   * sheet for details
   * @param  rpm        target motor rpm, preferrable 1-200
   * @param  steps      motor steps, usually 200 steps per revolution
   */
  A4988Device(PI_PIN        step_pin,
              PI_PIN        dir_pin,
              PI_PIN        enable_pin,
              double        rpm = 200.0,
              stepper::step steps = 200,
              PI_PIN        ms1_pin = PI_UNDEF_PIN,
              PI_PIN        ms2_pin = PI_UNDEF_PIN,
              PI_PIN        ms3_pin = PI_UNDEF_PIN);
  /**
   * A4988Device Destructor
   *
   * Close the A4988Device that has been initialized
   */
  virtual ~A4988Device() = default;
  /**
   * Set stepper microsteps
   *
   * @param microsteps microsteps to set
   */
  virtual void microsteps(const stepper::step& microsteps) override;
  /**
   * Get MS1 DigitalOutputDevice that has been initialized
   *
   * @return shared_ptr of DigitalOutputDevice
   */
  const std::shared_ptr<DigitalOutputDevice>& ms1_device() const {
    return ms1_device_;
  }
  /**
   * Get MS2 DigitalOutputDevice that has been initialized
   *
   * @return shared_ptr of DigitalOutputDevice
   */
  const std::shared_ptr<DigitalOutputDevice>& ms2_device() const {
    return ms2_device_;
  }
  /**
   * Get MS3 DigitalOutputDevice that has been initialized
   *
   * @return shared_ptr of DigitalOutputDevice
   */
  const std::shared_ptr<DigitalOutputDevice>& ms3_device() const {
    return ms3_device_;
  }
  /**
   * Get Step GPIO pin
   *
   * @return step GPIO pin
   */
  const PI_PIN& ms1_pin() const { return ms1_pin_; }
  /**
   * Get Direction GPIO pin
   *
   * @return direction GPIO pin
   */
  const PI_PIN& ms2_pin() const { return ms2_pin_; }
  /**
   * Get Enable GPIO pin
   *
   * @return enable GPIO pin
   */
  const PI_PIN& ms3_pin() const { return ms3_pin_; }
  /**
   * Get A4988 Stepper max microsteps
   *
   * @return max microsteps of A4988
   */
  static const stepper::step max_microsteps() { return max_microsteps_; }
  /**
   * Get A4988 Stepper Microstep table
   *
   * @return ms table of A4988 stepper
   */
  static const stepper::step* ms_table() { return ms_table_; }
  /**
   * Get A4988 Stepper Microstep table
   *
   * @return ms table of A4988 stepper
   */
  static const size_t ms_table_size() { return sizeof(ms_table_); }

 protected:
  /*
   * Microstepping resolution truth table (Page 6 of A4988 pdf)
   *
   * 0bMS3,MS2,MS1 for 1,2,4,8,16 microsteps
   */
  static const stepper::step ms_table_[];
  /*
   * Max microsteps of A4988
   *
   * Constant value = 0b1000 = 016
   */
  static const stepper::step max_microsteps_;
  /**
   * MS1 GPIO pin
   */
  const PI_PIN ms1_pin_;
  /**
   * MS2 GPIO pin */
  const PI_PIN ms2_pin_;
  /**
   * MS3 GPIO pin
   */
  const PI_PIN ms3_pin_;
  /**
   * MS1 digital output device
   */
  const std::shared_ptr<DigitalOutputDevice> ms1_device_;
  /**
   * MS2 digital output device
   */
  const std::shared_ptr<DigitalOutputDevice> ms2_device_;
  /**
   * MS3 digital output device
   */
  const std::shared_ptr<DigitalOutputDevice> ms3_device_;
};
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_DIGITAL_STEPPER_A4988_HPP_
