#include "device.hpp"

#include "shift_register.hpp"

NAMESPACE_BEGIN

namespace device {

namespace impl {
const PI_PIN ShiftRegisterDeviceImpl::cascade_num = 16;

ShiftRegisterDeviceImpl::ShiftRegisterDeviceImpl(
    PI_PIN                    latch_pin,
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

ATM_STATUS ShiftRegisterDeviceImpl::write(const byte&           pin,
                                          const digital::value& level) {
  if (pin < 0 && pin >= ShiftRegisterDeviceImpl::cascade_num) {
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

void ShiftRegisterDeviceImpl::shift_out(const byte& value) const {
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

void ShiftRegisterDeviceImpl::bit_write(byte&                 value,
                                        byte                  bit,
                                        const digital::value& bit_value) {
  if (bit_value == digital::value::high) {
    value |= (1UL << bit);
  } else {
    value &= ~(1UL << bit);
  }
}

bool ShiftRegisterDeviceImpl::active() const {
  return latch_device()->active() && clock_device()->active() &&
         data_device()->active();
}

ShiftRegisterImpl::ShiftRegisterImpl(PI_PIN                    latch_pin,
                                     PI_PIN                    clock_pin,
                                     PI_PIN                    data_pin,
                                     shift_register::bit_order order)
    : ShiftRegisterDeviceImpl{latch_pin, clock_pin, data_pin, order} {
  massert(active(), "sanity");
}

ATM_STATUS ShiftRegisterImpl::assign(const std::string& id,
                                     const byte&        pin,
                                     const bool&        active_state) {
  massert(container_.count(id) == 0, "device id must be unique");
  if (container_.count(id) > 0) {
    return ATM_ERR;
  }
  LOG_DEBUG("ShiftRegisterImpl::assign device with key {} and bit {}", id,
            static_cast<int>(pin));
  container_[id] = {pin, active_state};
  return ATM_OK;
}

ATM_STATUS ShiftRegisterImpl::write(const std::string&    id,
                                    const digital::value& level) {
  if (auto current_metadata = get(id)) {
    const auto& [address, active_state] = *current_metadata;
    LOG_DEBUG("Write ShiftRegister with address {} active_state {} level {}",
              address, active_state, level);
    if (active_state) {
      return ShiftRegisterDeviceImpl::write(address, level);
    }
    // invert output
    return ShiftRegisterDeviceImpl::write(address, level == digital::value::high
                                                       ? digital::value::low
                                                       : digital::value::high);
  }

  return ATM_ERR;
}

std::optional<ShiftRegisterImpl::metadata> ShiftRegisterImpl::get(
    const std::string& id) const {
  try {
    return container_.at(id);
  } catch ([[maybe_unused]] const std::out_of_range& e) {
    return {};
  }
}

bool ShiftRegisterImpl::exist(const std::string& id) const {
  try {
    [[maybe_unused]] auto instance = container_.at(id);
    return true;
  } catch ([[maybe_unused]] const std::out_of_range& e) {
    return false;
  }
}
}  // namespace impl
}  // namespace device

NAMESPACE_END