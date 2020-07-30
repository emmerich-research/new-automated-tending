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
enum class status { empty, normal, full };
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
   * @param  bottom_pin   gpio pin, see Raspberry GPIO pinout for details
   * @param  top_pin  gpio pin, see Raspberry GPIO pinout for details
   * @param  bottom_active_state    bottom active state (reversed or not)
   * @param  top_active_state   top active state (reversed or not)
   */
  FloatDevice(PI_PIN bottom_pin,
              PI_PIN top_pin,
              bool   bottom_active_state = true,
              bool   top_active_state = true);
  /**
   * FloatDevice Destructor
   *
   * Close the ShiftRegisterImpl that has been initialized
   */
  virtual ~FloatDevice();
  /**
   * Get Bottom GPIO pin
   *
   * @return bottom GPIO pin
   */
  inline const PI_PIN& bottom_pin() const { return bottom_pin_; }
  /**
   * Get Top float GPIO pin
   *
   * @return trigger GPIO pin
   */
  inline const PI_PIN& top_pin() const { return top_pin_; }
  /**
   * Get Bottom float DigitalInputDevice that has been initialized
   *
   * @return shared_ptr of DigitalInputDevice
   */
  inline const std::shared_ptr<DigitalInputDevice>& bottom_device() const {
    return bottom_device_;
  }
  /**
   * Get Top float DigitalOutputDevice that has been initialized
   *
   * @return shared_ptr of DigitalInputDevice
   */
  inline const std::shared_ptr<DigitalInputDevice>& top_device() const {
    return top_device_;
  }

 protected:
  /**
   * Bottom float pin
   */
  const PI_PIN bottom_pin_;
  /**
   * Top float pin
   */
  const PI_PIN top_pin_;
  /**
   * Bottom float digital input device
   */
  const std::shared_ptr<DigitalInputDevice> bottom_device_;
  /**
   * Top float digital input device
   */
  const std::shared_ptr<DigitalInputDevice> top_device_;
};
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_FLOAT_SENSOR_HPP_
