#ifndef LIB_DEVICE_DIGITAL_DIGITAL_HPP_
#define LIB_DEVICE_DIGITAL_DIGITAL_HPP_

/** @file digital.hpp
 *  @brief Digital device class definition
 *
 * Digital device using GPIO
 */

#include <optional>
#include <string>
#include <type_traits>

#include <libalgo/algo.hpp>
#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace device {
// forward declaration
namespace digital {
enum class mode;
}

template <digital::mode Mode>
class DigitalDevice;
// end of forward declaration

namespace digital {
enum class mode {
  input,
  output,
};

enum class value {
  low,
  high,
};
}  // namespace digital

/** digital::mode::output mode specific implementation of device::DigitalDevice
 */
using DigitalOutputDevice = DigitalDevice<digital::mode::output>;

/** digital::mode::input mode specific implementation of device::DigitalDevice
 */
using DigitalInputDevice = DigitalDevice<digital::mode::input>;

/** device::DigitalOutputDevice registry singleton class using
 * algo::InstanceRegistry
 */
using DigitalOutputDeviceRegistry = algo::InstanceRegistry<DigitalOutputDevice>;

/** device::DigitalInputDevice registry singleton class using
 * algo::InstanceRegistry
 */
using DigitalInputDeviceRegistry = algo::InstanceRegistry<DigitalInputDevice>;

/**
 * @brief Digital Device implementation.
 *
 * DigitalDevice will instantiate GPIO Device
 *
 * @tparam Mode digital device mode can be input and output
 *
 * @author Ray Andrew
 * @date   May 2020
 */
template <digital::mode Mode>
class DigitalDevice : public StackObj {
  template <class DigitalDevice>
  template <typename... Args>
  friend ATM_STATUS
  StaticObj<algo::impl::InstanceRegistryImpl<DigitalDevice>>::create(
      Args&&... args);

 public:
  /**
   * Create shared_ptr<DigitalDevice>
   *
   * Pass every args to DigitalDevice()
   *
   * @param args arguments that will be passed to DigitalDevice()
   */
  MAKE_STD_SHARED(DigitalDevice<Mode>)
  /**
   * Create unique_ptr<DigitalDevice>
   *
   * Pass every args to DigitalDevice()
   *
   * @param args arguments that will be passed to DigitalDevice()
   */
  MAKE_STD_UNIQUE(DigitalDevice<Mode>)
  /**
   * Check if pin is active or not
   *
   * @return active status of GPIO pin
   */
  bool active() const { return active_; }
  /**
   * Pull up at specified pin
   */
  ATM_STATUS pull_up();
  /**
   * Pull down at specified pin
   */
  ATM_STATUS pull_down();
  /**
   * Write the HIGH/LOW data to GPIO via Pigpio lib
   *
   * Only ENABLE if device mode is OUTPUT
   *
   * @param  level HIGH/LOW
   *
   * @return ATM_OK or ATM_ERR, but not both
   */
  template <digital::mode Mode_ = Mode,
            typename = std::enable_if_t<Mode_ == digital::mode::output>>
  ATM_STATUS write(const digital::value& level);
  /**
   * Read the HIGH/LOW data from GPIO via Pigpio lib
   *
   * Only ENABLE if device mode is INPUT
   *
   * @return digital::value HIGH or LOW
   */
  template <digital::mode Mode_ = Mode,
            typename = std::enable_if_t<Mode_ == digital::mode::input>>
  const std::optional<digital::value> read() const;
  /**
   * Read the HIGH/LOW data from GPIO via Pigpio lib
   *
   * Failed value will be denoted as "falsy"
   *
   * HIGH = true
   * LOW  = false
   *
   * @return ATM_OK or ATM_ERR, but not both
   */
  template <digital::mode Mode_ = Mode,
            typename = std::enable_if_t<Mode_ == digital::mode::input>>
  bool read_bool() const;
  /**
   * Get GPIO pin that has been initialized
   *
   * @return gpio pin
   */
  inline unsigned char pin() const { return pin_; }
  /**
   * Get current device mode of GPIO pin
   *
   * @return device mode
   */
  inline digital::mode mode() const { return mode_; }
  /**
   * Get current device mode of GPIO pin
   *
   * @return device mode
   */
  inline operator digital::mode() const { return mode_; }
  /**
   * Get string representation of current device mode of GPIO pin
   *
   * @return string representation of device mode
   */
  inline operator const char*() const { return get_mode(mode_); }
  /**
   * Set current active state of GPIO pin
   *
   * Can be TRUE means HIGH or FALSE means LOW
   */
  void active_state(bool active_state);

  /**
   * Get current active state of GPIO pin
   *
   * @return TRUE/FALSE
   */
  inline bool active_state() const { return active_state_; }

 private:
  /**
   * Read single byte to device associated with handle via Pigpio
   * lib
   *
   * @param  mode  device mode
   *
   * @return string representation of device mode
   */
  static inline const char* get_mode(const digital::mode& mode) {
    if (mode == digital::mode::input) {
      return "input";
    } else {
      return "output";
    }
  }

  template <digital::mode Mode_ = Mode,
            typename = std::enable_if_t<Mode_ == digital::mode::output>>
  static const int process_value(const digital::value& value,
                                 const bool            active_state);

  template <digital::mode Mode_ = Mode,
            typename = std::enable_if_t<Mode_ == digital::mode::input>>
  static const digital::value process_value(const int& value,
                                            const bool active_state);

 protected:
  /**
   * DigitalDevice Constructor
   *
   * Initialize the digital device by opening GPIO
   *
   * @param  pin gpio pin, see Raspberry GPIO pinout for details
   * @param  active_state whether the pin active state is reversed or not
   * @param  pull         whether the pin is pulled up, down, or off (default
   * down)
   */
  DigitalDevice(PI_PIN        pin,
                const bool&   active_state = true,
                const PI_PUD& pull = PI_PUD_DOWN);
  /**
   * DigitalDevice Destructor
   *
   * Close the DigitalDevice that has been initialized
   */
  virtual ~DigitalDevice() = default;

 protected:
  /**
   * GPIO pin
   */
  const PI_PIN pin_;
  /**
   * Device mode
   */
  const digital::mode mode_;
  /**
   * Active state
   */
  bool active_state_;
  /**
   * Active status
   *
   * Will become false if only initialize with PI_UNDEF_PIN
   **/
  bool active_;
};
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_DIGITAL_DIGITAL_HPP_
