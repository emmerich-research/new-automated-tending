#include "pwm.hpp"

#include "gpio.hpp"

NAMESPACE_BEGIN

namespace device {
PWMDevice::PWMDevice(PI_PIN pin, const bool& active_state)
    : DigitalOutputDevice{pin, active_state} {
  DEBUG_ONLY_DEFINITION(obj_name_ = fmt::format("PWMDevice pin {}", pin));

  DEBUG_ONLY(LOG_DEBUG("Initializing PWMDevice using GPIO with pin {}", pin));
}

PWMDevice::~PWMDevice() {}

ATM_STATUS PWMDevice::duty_cycle(unsigned int duty_cycle) {
  PI_RES res = gpioPWM(pin(), duty_cycle);

  if (res == PI_OK) {
    return ATM_OK;
  }

  LOG_DEBUG(
      "[FAILED] PWMDevice::duty_cycle with pin {}, failed to set value of pwm "
      "duty cycle to {}, result = {}",
      pin(), duty_cycle, res);
  return ATM_ERR;
}

std::optional<unsigned int> PWMDevice::duty_cycle() const {
  PI_RES res = gpioGetPWMdutycycle(pin());

  if (res == PI_BAD_USER_GPIO || res == PI_NOT_PWM_GPIO) {
    LOG_DEBUG(
        "[FAILED] PWMDevice::duty_cycle with pin {}, failed to get current "
        "duty cycle, result = {}",
        pin(), res);
    return {};
  }

  return static_cast<unsigned int>(res);
}

ATM_STATUS PWMDevice::range(unsigned int range) {
  PI_RES res = gpioSetPWMrange(pin(), range);

  if (res == PI_BAD_USER_GPIO || res == PI_BAD_DUTYRANGE) {
    LOG_DEBUG(
        "[FAILED] PWMDevice::duty_cycle with pin {}, failed to set value of "
        "pwm duty cycle range to {}, result = {}",
        pin(), range, res);
    return {};
  }

  return ATM_OK;
}

std::optional<unsigned int> PWMDevice::range() const {
  PI_RES res = gpioGetPWMrange(pin());

  if (res == PI_BAD_USER_GPIO || res == PI_BAD_DUTYRANGE) {
    LOG_DEBUG(
        "[FAILED] PWMDevice::range with pin {}, failed to get current "
        "duty cycle range, result = {}",
        pin(), res);
    return {};
  }
  return static_cast<unsigned int>(res);
}

std::optional<unsigned int> PWMDevice::real_range() const {
  PI_RES res = gpioGetPWMrealRange(pin());

  if (res == PI_BAD_USER_GPIO) {
    LOG_DEBUG(
        "[FAILED] PWMDevice::real_range with pin {}, failed to get current "
        "duty cycle real range, result = {}",
        pin(), res);
    return {};
  }

  return static_cast<unsigned int>(res);
}

ATM_STATUS PWMDevice::frequency(unsigned int frequency) {
  PI_RES res = gpioSetPWMfrequency(pin(), frequency);

  if (res == PI_BAD_USER_GPIO) {
    LOG_DEBUG(
        "[FAILED] PWMDevice::frequency with pin {}, failed to set value of "
        "pwm frequency to {}, result = {}",
        pin(), frequency, res);

    return ATM_ERR;
  }

  return ATM_OK;
}

std::optional<unsigned int> PWMDevice::frequency() const {
  PI_RES res = gpioGetPWMfrequency(pin());

  if (res == PI_BAD_USER_GPIO) {
    LOG_DEBUG(
        "[FAILED] PWMDevice::frequency with pin {}, failed to get current "
        "pwm frequency, result = {}",
        pin(), res);
    return {};
  }

  return static_cast<unsigned int>(res);
}

ATM_STATUS PWMDevice::hardware(unsigned int frequency,
                               unsigned int duty_cycle) {
  PI_RES res = gpioHardwarePWM(pin(), frequency, duty_cycle);

  if (res == PI_BAD_USER_GPIO) {
    LOG_DEBUG(
        "[FAILED] PWMDevice::hardware with pin {}, failed to set hardware pwm "
        "with frequency {} and duty_cycle {}, result = {}",
        pin(), frequency, duty_cycle, res);
    return ATM_ERR;
  }

  return ATM_OK;
}
}  // namespace device

NAMESPACE_END
