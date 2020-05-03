#ifndef LIB_DEVICE_DIGITAL_DIGITAL_HPP_
#define LIB_DEVICE_DIGITAL_DIGITAL_HPP_

#include <string>
#include <type_traits>

#include <libalgo/algo.hpp>
#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace device {

namespace digital {
enum class device_mode {
  INPUT,
  OUTPUT,
};

enum class device_output {
  LOW,
  HIGH,
};

inline const std::string getDeviceModeString(const device_mode& mode) {
  if (mode == device_mode::INPUT) {
    return "input";
  } else {
    return "output";
  }
}
}  // namespace digital

template <digital::device_mode Mode>
class DigitalDevice : public StackObj {
 protected:
  DigitalDevice(unsigned char pin);
  virtual ~DigitalDevice() = default;

  template <typename = std::enable_if_t<digital::device_mode::OUTPUT == Mode>>
  inline ATM_STATUS write(const digital::device_output& level);

  template <typename = std::enable_if_t<digital::device_mode::INPUT == Mode>>
  inline digital::device_output read() const;

  inline int                  get_pin() const { return pin_; }
  inline digital::device_mode get_mode() const { return Mode; }

 protected:
  const unsigned char pin_;
};
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_DIGITAL_DIGITAL_HPP_
