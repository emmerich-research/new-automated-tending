#ifndef LIB_DEVICE_DIGITAL_STEPPER_A4988_INLINE_HPP_
#define LIB_DEVICE_DIGITAL_STEPPER_A4988_INLINE_HPP_

#include "A4988.hpp"

NAMESPACE_BEGIN

namespace device {
template <stepper::speed Speed>
const stepper::step A4988Device<Speed>::ms_table_[] = {0b000, 0b001, 0b010,
                                                       0b011, 0b111};

template <stepper::speed Speed>
const stepper::step A4988Device<Speed>::max_microsteps_ = 0b10000;

template <stepper::speed Speed>
A4988Device<Speed>::A4988Device(PI_PIN        step_pin,
                                PI_PIN        dir_pin,
                                PI_PIN        enable_pin,
                                double        rpm,
                                stepper::step steps,
                                PI_PIN        ms1_pin,
                                PI_PIN        ms2_pin,
                                PI_PIN        ms3_pin)
    : impl::StepperDeviceImpl<Speed>{step_pin, dir_pin, enable_pin, rpm, steps},
      ms1_pin_{ms1_pin},
      ms2_pin_{ms2_pin},
      ms3_pin_{ms3_pin},
      ms1_device_{DigitalOutputDevice::create(ms1_pin)},
      ms2_device_{DigitalOutputDevice::create(ms2_pin)},
      ms3_device_{DigitalOutputDevice::create(ms3_pin)} {}

template <stepper::speed Speed>
void A4988Device<Speed>::microsteps(const stepper::step& microsteps) {
  if (microsteps < 0 && microsteps > max_microsteps()) {
    return;
  }

  impl::StepperDeviceImpl<Speed>::microsteps(microsteps);

  if (!ms1_device()->active() || !ms2_device()->active() ||
      !ms3_device()->active()) {
    return;
  }

  const stepper::step* table = ms_table();
  size_t               table_size = ms_table_size();

  unsigned short i = 0;
  while (i < table_size) {
    if (impl::StepperDeviceImpl<Speed>::microsteps() & (1 << i)) {
      const stepper::step mask = table[i];
      ms3_device()->write(mask & 4 ? digital::value::high
                                   : digital::value::low);
      ms2_device()->write(mask & 2 ? digital::value::high
                                   : digital::value::low);
      ms1_device()->write(mask & 1 ? digital::value::high
                                   : digital::value::low);
      break;
    }
    ++i;
  }
}
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_DIGITAL_STEPPER_A4988_INLINE_HPP_
