#ifndef LIB_DEVICE_DIGITAL_DIGITAL_INLINE_HPP_
#define LIB_DEVICE_DIGITAL_DIGITAL_INLINE_HPP_

#include "digital/digital.hpp"

#include "gpio.hpp"

NAMESPACE_BEGIN

namespace device {
template <digital::device_mode Mode>
DigitalDevice<Mode>::DigitalDevice(unsigned char pin) : pin_{pin}, mode_{Mode} {
  DEBUG_ONLY(obj_name_ = fmt::format("{}{} pin {}", get_device_mode(Mode),
                                     "DigitalDevice", pin));
  int res = gpioSetMode(
      pin_, mode_ == digital::device_mode::INPUT ? PI_INPUT : PI_OUTPUT);

  if (res != PI_OK) {
  }
}

template <digital::device_mode Mode>
template <typename>
ATM_STATUS DigitalDevice<Mode>::write(const digital::device_output& level) {
  return ATM_OK;
}

template <digital::device_mode Mode>
template <typename>
const digital::device_output DigitalDevice<Mode>::read() const {
  return digital::device_output::LOW;
}
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_DIGITAL_DIGITAL_INLINE_HPP_
