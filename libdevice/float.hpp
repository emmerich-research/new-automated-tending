#ifndef LIB_DEVICE_FLOAT_SENSOR_HPP_
#define LIB_DEVICE_FLOAT_SENSOR_HPP_

/** @file float_sensor.hpp
 *  @brief Float sensor class definition
 *
 * Float sensor device using GPIO
 */

#include <libalgo/algo.hpp>
#include <libcore/core.hpp>

#include "gpio.hpp"

#include "digital.hpp"

NAMESPACE_BEGIN

namespace device {
// forward declaration
class FloatDevice;

namespace float_sensor {
enum class status { high, low };
}

/** device::FloatDevice registry singleton class using
 * algo::InstanceRegistry
 */
using FloatDeviceRegistry = algo::InstanceRegistry<FloatDevice>;

/**
 * @brief Float Sensor Device implementation.
 *
 * Float Sensor Device implementation using device::DigitalOutputDevice
 *
 * @author Ray Andrew
 * @date   July 2020
 */
class FloatDevice : public StackObj {
 public:
  /**
   * Create shared_ptr<FloatDevice>
   *
   * Pass every args to FloatDevice()
   *
   * @param args arguments that will be passed to FloatDevice()
   */
  MAKE_STD_SHARED(FloatDevice)
  /**
   * Get status from float sensor
   *
   * @return float sensor status
   */
  float_sensor::status read() const;
  /**
   * Get active status
   *
   * @return active status
   */
  bool active() const;

 private:
  /**
   * FloatDevice Constructor
   *
   * Initialize the float sensor device by opening GPIO pins
   *
   * @param  pin   gpio pin, see Raspberry GPIO pinout for details
   * @param  active_state    bottom active state (reversed or not)
   */
  FloatDevice(PI_PIN pin, bool active_state = true);
  /**
   * FloatDevice Destructor
   *
   * Close the ShiftRegisterImpl that has been initialized
   */
  virtual ~FloatDevice();
  /**
   * Get float pin
   *
   * @return float GPIO pin
   */
  inline const PI_PIN& pin() const { return pin_; }
  /**
   * Get float DigitalOutputDevice that has been initialized
   *
   * @return shared_ptr of DigitalInputDevice
   */
  inline const std::shared_ptr<DigitalInputDevice>& device() const {
    return device_;
  }

 protected:
  /**
   * Float pin
   */
  const PI_PIN pin_;
  /**
   * Float digital input device
   */
  const std::shared_ptr<DigitalInputDevice> device_;
};
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_FLOAT_SENSOR_HPP_
