#ifndef LIB_DEVICE_DIGITAL_DIGITAL_HPP_
#define LIB_DEVICE_DIGITAL_DIGITAL_HPP_

/** @file digital.hpp
 *  @brief Digital device class definition
 *
 * Digital device using GPIO
 */

#include <string>
#include <type_traits>

#include <libalgo/algo.hpp>
#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace device {
// forward declaration
namespace digital {
enum class device_mode;
}

template <digital::device_mode Mode>
class DigitalDevice;
// end of forward declaration

namespace digital {
enum class device_mode {
  INPUT,
  OUTPUT,
};

enum class device_output {
  LOW,
  HIGH,
  ERROR,
};
}  // namespace digital

/** device::DigitalDevice registry singleton class using algo::InstanceRegistry
 */
template <digital::device_mode Mode>
using DigitalDeviceRegistry = algo::InstanceRegistry<DigitalDevice<Mode>>;

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
template <digital::device_mode Mode>
class DigitalDevice : public StackObj {
  template <class DigitalDevice>
  template <typename... Args>
  friend ATM_STATUS
  StaticObj<algo::impl::InstanceRegistryImpl<DigitalDevice>>::create(
      Args&&... args);

 public:
  MAKE_STD_SHARED(DigitalDevice<Mode>)
  /**
   * Create shared_ptr<DigitalDevice>
   *
   * Pass every args to DigitalDevice()
   *
   * @param args arguments that will be passed to DigitalDevice()
   */
  // template <typename... Args>
  // inline static auto create(Args&&... args) {
  //   return
  //   std::make_shared<DigitalDevice<Mode>>(std::forward<Args>(args)...);
  // }
  /**
   * Write the HIGH/LOW data to GPIO via Pigpio lib
   *
   * Only ENABLE if device mode is OUTPUT
   *
   * @param  level HIGH/LOW
   *
   * @return ATM_OK or ATM_ERR, but not both
   */
  template <digital::device_mode Mode_ = Mode,
            typename = std::enable_if_t<Mode_ == digital::device_mode::OUTPUT>>
  ATM_STATUS write(const digital::device_output& level);
  /**
   * Read the HIGH/LOW data from GPIO via Pigpio lib
   *
   * Only ENABLE if device mode is INPUT
   *
   * @return ATM_OK or ATM_ERR, but not both
   */
  template <digital::device_mode Mode_ = Mode,
            typename = std::enable_if_t<Mode_ == digital::device_mode::INPUT>>
  const digital::device_output read() const;
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
  inline digital::device_mode mode() const { return mode_; }
  /**
   * Get current device mode of GPIO pin
   *
   * @return device mode
   */
  inline operator digital::device_mode() const { return mode_; }
  /**
   * Get string representation of current device mode of GPIO pin
   *
   * @return string representation of device mode
   */
  inline operator const char*() const { return get_device_mode(mode_); }

 private:
  /**
   * Read single byte to device associated with handle via Pigpio
   * lib
   *
   * @param  mode  device mode
   *
   * @return string representation of device mode
   */
  static inline const char* get_device_mode(const digital::device_mode& mode) {
    if (mode == digital::device_mode::INPUT) {
      return "input";
    } else {
      return "output";
    }
  }

 protected:
  /**
   * DigitalDevice Constructor
   *
   * Initialize the digital device by opening GPIO
   *
   * @param  pin gpio pin, see Raspberry GPIO pinout for details
   */
  DigitalDevice(unsigned char pin);

  /**
   * DigitalDevice Destructor
   *
   * Close the GPIO that has been initialized
   */
  virtual ~DigitalDevice() = default;

 protected:
  /**
   * GPIO pin
   */
  const unsigned char pin_;
  /**
   * Device mode
   */
  const digital::device_mode mode_;
};
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_DIGITAL_DIGITAL_HPP_
