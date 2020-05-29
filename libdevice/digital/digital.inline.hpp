#ifndef LIB_DEVICE_DIGITAL_DIGITAL_INLINE_HPP_
#define LIB_DEVICE_DIGITAL_DIGITAL_INLINE_HPP_

#include "digital/digital.hpp"

#include "gpio.hpp"

NAMESPACE_BEGIN

namespace device {
template <digital::mode Mode>
DigitalDevice<Mode>::DigitalDevice(unsigned char pin)
    : pin_{pin}, mode_{Mode}, active_state_{true} {
  DEBUG_ONLY(obj_name_ =
                 fmt::format("DigitalDevice<{}> pin {}", get_mode(Mode), pin));

  LOG_DEBUG("Initializing DigitalDevice<{}> using GPIO with pin {}",
            get_mode(Mode), pin);

  PI_RES res =
      gpioSetMode(pin_, mode_ == digital::mode::INPUT ? PI_INPUT : PI_OUTPUT);

  if (res != PI_OK) {
    LOG_DEBUG("[FAILED] Initializing DigitalDevice<{}> using GPIO with pin {}",
              get_mode(Mode), pin);
    return;
  }
}

template <digital::mode Mode>
template <digital::mode Mode_, typename>
ATM_STATUS DigitalDevice<Mode>::write(const digital::value& level) {
  PI_RES res;
  switch (level) {
    case digital::value::LOW:
    case digital::value::HIGH:
      res = gpioWrite(pin_, process_value(level, active_state()));
      break;
    default:
      LOG_DEBUG(
          "[FAILED] DigitalDevice<{}> with pin {}, message = invalid level "
          "value",
          get_mode(Mode), pin_);
  }

  if (res == PI_OK) {
    return ATM_OK;
  }

  LOG_DEBUG("[FAILED] DigitalDevice<{}>::write with pin {}, result = {}",
            get_mode(Mode), pin_, res);
  return ATM_ERR;
}

template <digital::mode Mode>
template <digital::mode Mode_, typename>
const digital::value DigitalDevice<Mode>::read() const {
  PI_RES res = gpioRead(pin_);

  if (res == PI_BAD_GPIO) {
    LOG_DEBUG("[FAILED] DigitalDevice<{}>::read with pin {}, result = {}",
              get_mode(Mode), pin_, res);
    return digital::value::ERROR;
  }

  return process_value(res, active_state());
}

template <digital::mode Mode>
template <digital::mode Mode_, typename>
const PI_RES DigitalDevice<Mode>::process_value(const digital::value& value,
                                                const bool active_state) {
  if (active_state) {
    if (value == digital::value::HIGH) {
      return PI_HIGH;
    } else {
      return PI_LOW;
    }
  }

  if (value == digital::value::HIGH) {
    return PI_LOW;
  } else {
    return PI_HIGH;
  }
}

template <digital::mode Mode>
template <digital::mode Mode_, typename>
const digital::value& DigitalDevice<Mode>::process_value(
    const PI_RES& value,
    const bool    active_state) {
  if (active_state) {
    if (value == PI_HIGH) {
      return digital::value::HIGH;
    } else {
      return digital::value::LOW;
    }
  }

  if (value == PI_HIGH) {
    return digital::value::LOW;
  } else {
    return digital::value::HIGH;
  }
}

template <digital::mode Mode>
void DigitalDevice<Mode>::active_state(bool active_state) {
  active_state_ = active_state;
}
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_DIGITAL_DIGITAL_INLINE_HPP_
