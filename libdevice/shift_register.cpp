#include "device.hpp"

#include "shift_register.hpp"

NAMESPACE_BEGIN

namespace device {

namespace impl {
const unsigned int ShiftRegisterDeviceImpl::cascade_num = 2;
const unsigned int ShiftRegisterDeviceImpl::shift_bits = 8;

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
      data_device_{DigitalOutputDevice::create(data_pin)},
      bits_{0} {
  DEBUG_ONLY(obj_name_ = "ShiftRegisterDevice");
  massert(active(), "sanity");
}

ShiftRegisterDeviceImpl::~ShiftRegisterDeviceImpl() {}

ATM_STATUS ShiftRegisterDeviceImpl::write(unsigned int          pin,
                                          const digital::value& level) {
  if (pin > cascade_num * shift_bits) {
    return ATM_ERR;
  }

  // turn off the output
  latch_device()->write(digital::value::low);

  // turn on the next highest bit in bits
  bit_write(bits(), pin, level);

  // shift the bits out
  shift_out(bits());

  // turn on the output
  latch_device()->write(digital::value::high);

  return ATM_OK;
}

void ShiftRegisterDeviceImpl::shift_out(unsigned int value) const {
  for (unsigned int i = 0; i < shift_bits * cascade_num; ++i) {
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

void ShiftRegisterDeviceImpl::bit_write(unsigned int&         value,
                                        unsigned int          bit,
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

ShiftRegisterImpl::~ShiftRegisterImpl() {}

ATM_STATUS ShiftRegisterImpl::assign(const std::string& id,
                                     unsigned int       pin,
                                     const bool&        active_state) {
  massert(container_.count(id) == 0, "device id must be unique");
  if (container_.count(id) > 0) {
    return ATM_ERR;
  }
  LOG_DEBUG("ShiftRegisterImpl::assign device with key {} and bit {}", id, pin);
  container_[id] = {pin, active_state};
  // enforce to write low
  write(id, digital::value::low);
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
