#ifndef LIB_DEVICE_DIGITAL_DIGITAL_INLINE_HPP_
#define LIB_DEVICE_DIGITAL_DIGITAL_INLINE_HPP_

#include "digital.hpp"

namespace device {
template <digital::device Mode>
DigitalDevice<Mode>::DigitalDevice(unsigned char pin) : pin_(pin) {
  DEBUG_ONLY(obj_name_ =
                 fmt::format("{}{} pin {}", digital::getDeviceMode(Mode),
                             "DigitalDevice", pin));
}
}  // namespace device

#endif  // LIB_DEVICE_DIGITAL_DIGITAL_INLINE_HPP_
