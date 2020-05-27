#ifndef LIB_DEVICE_DIGITAL_DIGITAL_HPP_
#define LIB_DEVICE_DIGITAL_DIGITAL_HPP_

#include <libcore/core.hpp>

#include <libalgo/algo.hpp>

NAMESPACE_BEGIN

namespace device {
// forward declaration
namespace digital {
enum class device_mode;
}

template <digital::device_mode Mode>
class DigitalDevice;

namespace digital {
enum class device_mode {
  INPUT,
  OUTPUT,
  PWM,
};

enum class device_output { LOW, HIGH };
}  // namespace digital

/** device::DigitalDevice registry singleton class using algo::InstanceRegistry
 */
template <digital::device_mode Mode>
using DigitalDeviceRegistry = algo::InstanceRegistry<DigitalDevice<Mode>>;

/**
 * @brief Instance Registry implementation.
 *        This is a class wrapper that should not be instantiated and accessed
 * publicly.
 *
 * Instance registry will create, hold, and destroy all the instances
 *
 * @tparam Mode digital device mode can be input, output, or pwm
 *
 * @author Ray Andrew
 * @date   April 2020
 */
template <digital::device_mode Mode>
class DigitalDevice {
 public:
  DigitalDevice(uint pin);
  virtual ~DigitalDevice() = default;

  template <typename = std::enable_if_t<Mode == digital::device_mode::OUTPUT>>
  void write(const digital::device_output& level);

  template <typename = std::enable_if_t<Mode == digital::device_mode::INPUT>>
  const digital::device_output read() const;

  inline int                  get_pin() const { return pin_; }
  inline digital::device_mode get_mode() const { return mode_; }
  inline const char* get_mode_str() const { return get_device_mode(mode_); }

  // virtual void set_mode(const device_mode& mode) = 0;

 private:
  static inline const char* get_device_mode(const digital::device_mode& mode) {
    if (mode == digital::device_mode::INPUT) {
      return "input";
    } else if (mode == digital::device_mode::OUTPUT) {
      return "output";
    } else {
      return "pwm";
    }
  }

 protected:
  const uint                 pin_;
  const digital::device_mode mode_;
};
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_DIGITAL_DIGITAL_HPP_
