#ifndef LIB_DEVICE_SHIFT_ULTRASONIC_HPP_
#define LIB_DEVICE_SHIFT_ULTRASONIC_HPP_

/** @file ultrasonic.hpp
 *  @brief Ultrasonic HC-SR04 Device class definition
 *
 * Ultrasonic HC-SR04 Device using GPIO
 */

#include <libalgo/algo.hpp>
#include <libcore/core.hpp>

#include "gpio.hpp"

#include "digital.hpp"

NAMESPACE_BEGIN

namespace device {
// forward declaration
class UltrasonicDevice;

/** device::UltrasonicDevice registry singleton class using
 * algo::InstanceRegistry
 */
using UltrasonicDeviceRegistry = algo::InstanceRegistry<UltrasonicDevice>;

/**
 * @brief Ultrasonic HC-SR04 Device implementation.
 *
 * Ultrasonic HC-SR04 Device implementation using
 * device::DigitalOutputDevice and device::DigitalOutputDevice
 *
 * @author Ray Andrew
 * @date   June 2020
 */
class UltrasonicDevice : public StackObj {
 public:
  /**
   * Create shared_ptr<UltrasonicDevice>
   *
   * Pass every args to UltrasonicDevice()
   *
   * @param args arguments that will be passed to UltrasonicDevice()
   */
  MAKE_STD_SHARED(UltrasonicDevice)
  /**
   * Get distance that is gotten from HC-SR04
   *
   * BEWARE: that this function can block the current thread!
   *
   * @return distance in cm
   */
  double distance() const;
  /**
   * Get active status
   *
   * @return active status
   */
  bool active() const;

 private:
  /**
   * UltrasonicDevice Constructor
   *
   * Initialize the shift register device by opening GPIO pins
   *
   * @param  echo_pin             gpio pin, see Raspberry GPIO pinout for
   * details
   * @param  trigger_pin          gpio pin, see Raspberry GPIO pinout for
   * details
   * @param  echo_active_state    echo active state (reversed or not)
   * @param  trigger_active_state echo active state (reversed or not)
   */
  UltrasonicDevice(PI_PIN echo_pin,
                   PI_PIN trigger_pin,
                   bool   echo_active_state = true,
                   bool   trigger_active_state = true);
  /**
   * UltrasonicDevice Destructor
   *
   * Close the UltrasonicDevice that has been initialized
   */
  virtual ~UltrasonicDevice();
  /**
   * Get Echo GPIO pin
   *
   * @return echo GPIO pin
   */
  inline const PI_PIN& echo_pin() const { return echo_pin_; }
  /**
   * Get Trigger GPIO pin
   *
   * @return trigger GPIO pin
   */
  inline const PI_PIN& trigger_pin() const { return trigger_pin_; }
  /**
   * Get Echo DigitalInputDevice that has been initialized
   *
   * @return shared_ptr of DigitalInputDevice
   */
  inline const std::shared_ptr<DigitalInputDevice>& echo_device() const {
    return echo_device_;
  }
  /**
   * Get Trigger DigitalOutputDevice that has been initialized
   *
   * @return shared_ptr of DigitalOutputDevice
   */
  inline const std::shared_ptr<DigitalOutputDevice>& trigger_device() const {
    return trigger_device_;
  }

 private:
  /**
   * Echo pin
   */
  const PI_PIN echo_pin_;
  /**
   * Trigger pin
   */
  const PI_PIN trigger_pin_;
  /**
   * Echo digital input device
   */
  const std::shared_ptr<DigitalInputDevice> echo_device_;
  /**
   * Trigger digital output device
   */
  const std::shared_ptr<DigitalOutputDevice> trigger_device_;
};
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_SHIFT_ULTRASONIC_HPP_
