#include "device.hpp"

#include "shift_register.hpp"

NAMESPACE_BEGIN

namespace device {
const PI_PIN ShiftRegisterDevice::cascade_num = 16;

ShiftRegisterDevice::ShiftRegisterDevice(PI_PIN                    latch_pin,
                                         PI_PIN                    clock_pin,
                                         PI_PIN                    data_pin,
                                         shift_register::bit_order order)
    : latch_pin_{latch_pin},
      clock_pin_{clock_pin},
      data_pin_{data_pin},
      order_{order},
      latch_device_{DigitalOutputDevice::create(latch_pin)},
      clock_device_{DigitalOutputDevice::create(clock_pin)},
      data_device_{DigitalOutputDevice::create(data_pin)} {
  DEBUG_ONLY(obj_name_ = "ShiftRegisterDevice");
  massert(latch_device()->active(), "sanity");
  massert(clock_device()->active(), "sanity");
  massert(data_device()->active(), "sanity");
}

ATM_STATUS ShiftRegisterDevice::write(const byte&           pin,
                                      const digital::value& level) {
  if (pin < 0 && pin >= ShiftRegisterDevice::cascade_num) {
    return ATM_ERR;
  }

  // bits to send
  byte bits = 0;

  // turn off the output so the pins don't
  // light up while the bits are being shifted in
  latch_device()->write(digital::value::low);

  // turn on the next highest bit in bits
  bit_write(bits, pin, level);

  // shift the bits out
  shift_out(bits);

  // turn on the output
  latch_device()->write(digital::value::high);

  return ATM_OK;
}

void ShiftRegisterDevice::shift_out(const byte& value) const {
  for (unsigned int i = 0; i < 8; i++) {
    if (order() == shift_register::bit_order::lsb) {
      data_device()->write(!!(value & (1 << i)) ? digital::value::high
                                                : digital::value::low);
    } else {
      data_device()->write(!!(value & (1 << (7 - i))) ? digital::value::high
                                                      : digital::value::low);
    }

    clock_device()->write(digital::value::high);
    clock_device()->write(digital::value::low);
  }
}

void ShiftRegisterDevice::bit_write(byte&                 value,
                                    byte                  bit,
                                    const digital::value& bit_value) {
  if (bit_value == digital::value::high) {
    value |= (1UL << bit);
  } else {
    value &= ~(1UL << bit);
  }
}
}  // namespace device

NAMESPACE_END
