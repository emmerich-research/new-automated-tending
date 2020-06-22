#ifndef LIB_DEVICE_DIGITAL_DIGITAL_INLINE_HPP_
#define LIB_DEVICE_DIGITAL_DIGITAL_INLINE_HPP_

#include "digital.hpp"

#include "gpio.hpp"

NAMESPACE_BEGIN

namespace device {
template <digital::mode Mode>
DigitalDevice<Mode>::DigitalDevice(PI_PIN        pin,
                                   const bool&   active_state,
                                   const PI_PUD& pull)
    : pin_{pin}, active_{true}, mode_{Mode}, active_state_{active_state} {
  DEBUG_ONLY(obj_name_ = fmt::format("DigitalDevice<{}> pin {} active_state {}",
                                     get_mode(Mode), pin, active_state));

  if (pin == PI_UNDEF_PIN) {
    active_ = false;
    return;
  }

  LOG_DEBUG("Initializing DigitalDevice<{}> using GPIO with pin {}",
            get_mode(Mode), pin);

  PI_RES res =
      gpioSetMode(pin_, mode_ == digital::mode::input ? PI_INPUT : PI_OUTPUT);

  if (res != PI_OK) {
    LOG_DEBUG("[FAILED] Initializing DigitalDevice<{}> using GPIO with pin {}",
              get_mode(Mode), pin);
    active_ = false;
    return;
  }

  // it is ok if fail as long as set mode is not fail
  if (pull == PI_PUD_DOWN) {
    (void)pull_down();
  } else {
    (void)pull_up();
  }
}

template <digital::mode Mode>
template <digital::mode Mode_, typename>
ATM_STATUS DigitalDevice<Mode>::write(const digital::value& level) {
  if (!active()) {
    LOG_DEBUG(
        "[FAILED] DigitalDevice<{}>::write with pin {}, device is not active!",
        get_mode(Mode), pin_);
    return ATM_ERR;
  }

  PI_RES res;
  switch (level) {
    case digital::value::low:
    case digital::value::high:
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
const std::optional<digital::value> DigitalDevice<Mode>::read() const {
  if (!active()) {
    LOG_DEBUG(
        "[FAILED] DigitalDevice<{}>::read with pin {}, device is not active!",
        get_mode(Mode), pin_);
    return {};
  }

  PI_RES res = gpioRead(pin_);

  if (res == PI_BAD_GPIO) {
    LOG_DEBUG("[FAILED] DigitalDevice<{}>::read with pin {}, result = {}",
              get_mode(Mode), pin_, res);
    return {};
  }

  return process_value(res, active_state());
}

template <digital::mode Mode>
template <digital::mode Mode_, typename>
bool DigitalDevice<Mode>::read_bool() const {
  if (!active()) {
    LOG_DEBUG(
        "[FAILED] DigitalDevice<{}>::read with pin {}, device is not active!",
        get_mode(Mode), pin_);
    return false;
  }

  auto result = read();

  if (result) {
    if (result == digital::value::high) {
      return true;
    } else {
      return false;
    }
  }

  return false;
}

template <digital::mode Mode>
template <digital::mode Mode_, typename>
PI_RES DigitalDevice<Mode>::process_value(const digital::value& value,
                                          const bool            active_state) {
  if (active_state) {
    if (value == digital::value::high) {
      return PI_HIGH;
    } else {
      return PI_LOW;
    }
  }

  if (value == digital::value::high) {
    return PI_LOW;
  } else {
    return PI_HIGH;
  }
}

template <digital::mode Mode>
template <digital::mode Mode_, typename>
digital::value DigitalDevice<Mode>::process_value(const PI_RES& value,
                                                  const bool    active_state) {
  if (active_state) {
    if (value == PI_HIGH) {
      return digital::value::high;
    } else {
      return digital::value::low;
    }
  }

  if (value == PI_HIGH) {
    return digital::value::low;
  } else {
    return digital::value::high;
  }
}

template <digital::mode Mode>
void DigitalDevice<Mode>::active_state(bool active_state) {
  active_state_ = active_state;
}

template <digital::mode Mode>
ATM_STATUS DigitalDevice<Mode>::pull_up() {
  ATM_STATUS res = gpioSetPullUpDown(pin_, PI_PUD_UP);

  if (res != PI_OK) {
    LOG_DEBUG(
        "[FAILED] Failed to PULL UP DigitalDevice<{}> using GPIO with "
        "pin {}",
        get_mode(Mode), pin());
  }

  return res;       
}

template <digital::mode Mode>
ATM_STATUS DigitalDevice<Mode>::pull_down() {
  ATM_STATUS res = gpioSetPullUpDown(pin_, PI_PUD_DOWN);

  if (res != PI_OK) {
    LOG_DEBUG(
        "[FAILED] Failed to PULL DOWN DigitalDevice<{}> using GPIO with "
        "pin {}",
        get_mode(Mode), pin());
  }

  return res;
}

template <digital::mode Mode>
ATM_STATUS DigitalDevice<Mode>::pull_off() {
  ATM_STATUS res = gpioSetPullUpDown(pin_, PI_PUD_OFF);

  if (res != PI_OK) {
    LOG_DEBUG(
        "[FAILED] Failed to PULL OFF DigitalDevice<{}> using GPIO with "
        "pin {}",
        get_mode(Mode), pin());
  }

  return res;
}
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_DIGITAL_DIGITAL_INLINE_HPP_
