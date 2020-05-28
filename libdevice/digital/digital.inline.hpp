#ifndef LIB_DEVICE_DIGITAL_DIGITAL_INLINE_HPP_
#define LIB_DEVICE_DIGITAL_DIGITAL_INLINE_HPP_

#include "digital/digital.hpp"

#include "gpio.hpp"

NAMESPACE_BEGIN

namespace device {
template <digital::device_mode Mode>
DigitalDevice<Mode>::DigitalDevice(unsigned char pin) : pin_{pin}, mode_{Mode} {
  DEBUG_ONLY(obj_name_ = fmt::format("DigitalDevice<{}> pin {}",
                                     get_device_mode(Mode), pin));

  LOG_DEBUG("Initializing DigitalDevice<{}> using GPIO with pin {}",
            get_device_mode(Mode), pin);

  int res = gpioSetMode(
      pin_, mode_ == digital::device_mode::INPUT ? PI_INPUT : PI_OUTPUT);

  if (res != PI_OK) {
    LOG_DEBUG("[FAILED] Initializing DigitalDevice<{}> using GPIO with pin {}",
              get_device_mode(Mode), pin);
    return;
  }
}

template <digital::device_mode Mode>
template <digital::device_mode Mode_, typename>
ATM_STATUS DigitalDevice<Mode>::write(const digital::device_output& level) {
  int res;
  switch (level) {
    case digital::device_output::LOW:
      res = gpioWrite(pin_, PI_LOW);
      break;
    case digital::device_output::HIGH:
      res = gpioWrite(pin_, PI_HIGH);
      break;
    default:
      LOG_DEBUG(
          "[FAILED] DigitalDevice<{}> with pin {}, message = invalid level "
          "value",
          get_device_mode(Mode), pin_);
  }

  if (res == PI_OK) {
    return ATM_OK;
  }

  LOG_DEBUG("[FAILED] DigitalDevice<{}>::write with pin {}, result = {}",
            get_device_mode(Mode), pin_, res);
  return ATM_ERR;
}

template <digital::device_mode Mode>
template <digital::device_mode Mode_, typename>
const digital::device_output DigitalDevice<Mode>::read() const {
  int res = gpioRead(pin_);

  if (res == PI_BAD_GPIO) {
    LOG_DEBUG("[FAILED] DigitalDevice<{}>::read with pin {}, result = {}",
              get_device_mode(Mode), pin_, res);
    return digital::device_output::ERROR;
  }

  return res == 0 ? digital::device_output::LOW : digital::device_output::HIGH;
}
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_DIGITAL_DIGITAL_INLINE_HPP_
