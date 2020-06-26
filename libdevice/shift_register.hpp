#ifndef LIB_DEVICE_SHIFT_REGISTER_HPP_
#define LIB_DEVICE_SHIFT_REGISTER_HPP_

/** @file shift_register.hpp
 *  @brief Shift Register class definition
 *
 * Shift Register Device using GPIO
 */

#include <libalgo/algo.hpp>
#include <libcore/core.hpp>

#include "gpio.hpp"

#include "digital.hpp"

NAMESPACE_BEGIN

namespace device {
// forward declaration
class ShiftRegisterDevice;
namespace shift_register {
enum class BitOrder;
}

using byte = unsigned char;

/** device::ShiftRegisterDevice registry singleton class using
 * algo::InstanceRegistry
 */
using ShiftRegisterRegistry = algo::InstanceRegistry<StepperDevice>;

namespace shift_register {
enum class bit_order {
  lsb /**< least significant bit */,
  msb /**< most significant bit */
};
}

/**
 * @brief Shift Register Device implementation.
 *
 * Shift Register Device implementation using
 * multiple device::DigitalOutputDevice
 *
 * @author Ray Andrew
 * @date   June 2020
 */
class ShiftRegisterDevice : public StackObj {
 public:
  /**
   * Create shared_ptr<ShiftRegisterDevice>
   *
   * Pass every args to ShiftRegisterDevice()
   *
   * @param args arguments that will be passed to ShiftRegisterDevice()
   */
  MAKE_STD_SHARED(ShiftRegisterDevice)

  /**
   * Write the HIGH/LOW data to ShiftRegisterDevice
   *
   * Taken from :
   * http://www.learningaboutelectronics.com/Articles/Cascade-shift-registers.php
   *
   * @param  pin   shift register pin/bit
   * @param  level HIGH/LOW
   *
   * @return ATM_OK or ATM_ERR, but not both
   */
  ATM_STATUS write(const byte& pin, const digital::value& level);

 protected:
  /**
   * ShiftRegisterDevice Constructor
   *
   * Initialize the shift register device by opening GPIO pins
   *
   * @param  latch_pin  gpio pin, see Raspberry GPIO pinout for details
   * @param  clock_pin  gpio pin, see Raspberry GPIO pinout for details
   * @param  data_pin   gpio pin, see Raspberry GPIO pinout for details
   * @param  order      order of bit, default MSB (Most Significant Bit)
   */
  ShiftRegisterDevice(
      PI_PIN                    latch_pin,
      PI_PIN                    clock_pin,
      PI_PIN                    data_pin,
      shift_register::bit_order order = shift_register::bit_order::msb);
  /**
   * ShiftRegisterDevice Destructor
   *
   * Close the ShiftRegisterDevice that has been initialized
   */
  virtual ~ShiftRegisterDevice() = default;
  /**
   * Get Latch DigitalOutputDevice that has been initialized
   *
   * @return shared_ptr of DigitalOutputDevice
   */
  inline const std::shared_ptr<DigitalOutputDevice>& latch_device() const {
    return latch_device_;
  }
  /**
   * Get Clock DigitalOutputDevice that has been initialized
   *
   * @return shared_ptr of DigitalOutputDevice
   */
  inline const std::shared_ptr<DigitalOutputDevice>& clock_device() const {
    return clock_device_;
  }
  /**
   * Get Data DigitalOutputDevice that has been initialized
   *
   * @return shared_ptr of DigitalOutputDevice
   */
  inline const std::shared_ptr<DigitalOutputDevice>& data_device() const {
    return data_device_;
  }
  /**
   * Get Latch GPIO pin
   *
   * @return latch GPIO pin
   */
  inline const PI_PIN& latch_pin() const { return latch_pin_; }
  /**
   * Get Clock GPIO pin
   *
   * @return clock GPIO pin
   */
  inline const PI_PIN& clock_pin() const { return clock_pin_; }
  /**
   * Get Data GPIO pin
   *
   * @return data GPIO pin
   */
  inline const PI_PIN& data_pin() const { return data_pin_; }
  /**
   * Get bit order
   *
   * @return bit order
   */
  inline const shift_register::bit_order& order() const { return order_; }

  /**
   * Shift out register
   *
   * @param value      value to set
   */
  void shift_out(const byte& value) const;

  /**
   * Bit Write
   * Taken from Arduino
   *
   * @param value      value to set
   * @param bit        which bit of the number to write, starting at 0 for the
   * least-significant (rightmost) bit.
   * @param bit_value  value to write to the bit
   *
   * @return change of value based on the bit_value
   */
  static void bit_write(byte& value, byte bit, const digital::value& bit_value);

 protected:
  /**
   * Shift register cascade number
   */
  static const PI_PIN cascade_num;
  /**
   * Bit order
   */
  const shift_register::bit_order order_;
  /**
   * Latch GPIO pin
   */
  const PI_PIN latch_pin_;
  /**
   * Clock GPIO pin
   */
  const PI_PIN clock_pin_;
  /**
   * Data GPIO pin
   */
  const PI_PIN data_pin_;
  /**
   * Latch digital output device
   */
  const std::shared_ptr<DigitalOutputDevice> latch_device_;
  /**
   * Clock digital output device
   */
  const std::shared_ptr<DigitalOutputDevice> clock_device_;
  /**
   * Data digital output device
   */
  const std::shared_ptr<DigitalOutputDevice> data_device_;
};
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_SHIFT_REGISTER_HPP_
