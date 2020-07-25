#ifndef LIB_DEVICE_SHIFT_REGISTER_HPP_
#define LIB_DEVICE_SHIFT_REGISTER_HPP_

/** @file shift_register.hpp
 *  @brief Shift Register class definition
 *
 * Shift Register Device using GPIO
 */

#include <libalgo/algo.hpp>
#include <libcore/core.hpp>

#include <optional>
#include <unordered_map>
#include <utility>

#include "gpio.hpp"

#include "digital.hpp"

NAMESPACE_BEGIN

namespace device {
// forward declaration
namespace impl {
class ShiftRegisterDeviceImpl;
class ShiftRegisterImpl;
}  // namespace impl
namespace shift_register {
enum class BitOrder;
}

namespace shift_register {
enum class bit_order {
  lsb /**< least significant bit */,
  msb /**< most significant bit */
};
}

/** impl::ShiftRegisterImpl singleton class using StaticObj */
using ShiftRegister = StaticObj<impl::ShiftRegisterImpl>;

namespace impl {
/**
 * @brief Shift Register Device implementation.
 *
 * Shift Register Device implementation using
 * multiple device::DigitalOutputDevice
 *
 * @author Ray Andrew
 * @date   June 2020
 */
class ShiftRegisterDeviceImpl : public StackObj {
 public:
  /**
   * Write the HIGH/LOW data to ShiftRegisterDeviceImpl
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
   * ShiftRegisterDeviceImpl Constructor
   *
   * Initialize the shift register device by opening GPIO pins
   *
   * @param  latch_pin  gpio pin, see Raspberry GPIO pinout for details
   * @param  clock_pin  gpio pin, see Raspberry GPIO pinout for details
   * @param  data_pin   gpio pin, see Raspberry GPIO pinout for details
   * @param  order      order of bit, default MSB (Most Significant Bit)
   */
  ShiftRegisterDeviceImpl(
      PI_PIN                    latch_pin,
      PI_PIN                    clock_pin,
      PI_PIN                    data_pin,
      shift_register::bit_order order = shift_register::bit_order::msb);
  /**
   * ShiftRegisterDeviceImpl Destructor
   *
   * Close the ShiftRegisterDeviceImpl that has been initialized
   */
  virtual ~ShiftRegisterDeviceImpl();
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
   */
  static void bit_write(byte& value, byte bit, const digital::value& bit_value);
  /**
   * Check if device is active or not
   *
   * @return active status of device
   */
  bool active() const;
  /**
   * Get bits with specified index
   *
   * @param idx index of bits
   *
   * @return bits with specified index
   */
  inline const byte& bits(unsigned int idx) const {
    massert(idx < cascade_num, "sanity");
    return bits_[idx];
  }
  /**
   * Get bits with specified index
   *
   * @param idx index of bits
   *
   * @return bits with specified index
   */
  inline byte& bits(unsigned int idx) {
    massert(idx < cascade_num, "sanity");
    return bits_[idx];
  }
  /**
   * Reset bits
   */
  void reset_bits();

 protected:
  /**
   * Shift register cascade number
   */
  static const unsigned int cascade_num;
  /**
   * Shift register bits
   */
  static const unsigned int shift_bits;
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
   * Bit order
   */
  const shift_register::bit_order order_;
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
  /**
   * Last bits of registers
   */
  byte* bits_;
};

/**
 * @brief Shift Register implementation.
 *
 * This will extend device::impl::ShiftRegisterDeviceImpl
 *
 * Shift Register implementation with virtual
 * registry to produce uniform API with InstanceRegistry
 *
 * Why is this important?
 * Devices should be registered with unique ID
 * Devices that connect to Shift Register themselves are
 * not an instance (not DigitalOutputDevice or etc)
 * This class is to simulate that we can call devices
 * that connected via ShiftRegister in the same manner
 * with other devices (DigitalOutputDevice, etc)
 *
 * @author Ray Andrew
 * @date   June 2020
 */
class ShiftRegisterImpl : public ShiftRegisterDeviceImpl {
  template <class ShiftRegisterImpl>
  template <typename... Args>
  friend ATM_STATUS StaticObj<ShiftRegisterImpl>::create(Args&&... args);

 public:
  /**
   * Connected devices metadata
   */
  typedef std::pair<byte, bool> metadata;
  /**
   * Register device with id and virtual pin in the
   * shift register
   *
   * @param id     unique identifier of device
   * @param pin    pin of device in the shift register
   *
   * @return status ATM_ERR or ATM_OK
   */
  ATM_STATUS assign(const std::string& id,
                    const byte&        pin,
                    const bool&        active_state = true);
  /**
   * Write the HIGH/LOW data to ShiftRegisterDeviceImpl
   *
   * @param  id    device unique id
   * @param  level HIGH/LOW
   *
   * @return ATM_OK or ATM_ERR, but not both
   */
  ATM_STATUS write(const std::string& id, const digital::value& level);
  /**
   * Check device with unique id
   *
   * @param  id    unique identifier of device
   *
   * @return exist or not
   */
  bool exist(const std::string& id) const;
  /**
   * Get pin/bit of device with unique id
   *
   * @param  id    unique identifier of device
   *
   * @return pin/bit of given id or fail
   */
  std::optional<metadata> get(const std::string& id) const;

 protected:
  /**
   * ShiftRegisterImpl Constructor
   *
   * Initialize the shift register device by opening GPIO pins
   *
   * @param  latch_pin  gpio pin, see Raspberry GPIO pinout for details
   * @param  clock_pin  gpio pin, see Raspberry GPIO pinout for details
   * @param  data_pin   gpio pin, see Raspberry GPIO pinout for details
   * @param  order      order of bit, default MSB (Most Significant Bit)
   */
  ShiftRegisterImpl(
      PI_PIN                    latch_pin,
      PI_PIN                    clock_pin,
      PI_PIN                    data_pin,
      shift_register::bit_order order = shift_register::bit_order::msb);
  /**
   * ShiftRegisterImpl Destructor
   *
   * Close the ShiftRegisterImpl that has been initialized
   */
  virtual ~ShiftRegisterImpl() override;

 protected:
  /**
   * "Instances"-like container for connected devices
   * with ShiftRegister
   */
  std::unordered_map<std::string, metadata> container_;
};
}  // namespace impl
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_SHIFT_REGISTER_HPP_
