#include "device.hpp"

#include "float.hpp"

NAMESPACE_BEGIN

namespace device {

FloatDevice::FloatDevice(PI_PIN pin, bool active_state)
    : pin_{pin}, device_{DigitalInputDevice::create(pin, active_state)} {
  DEBUG_ONLY_DEFINITION(
      obj_name_ = fmt::format("FloatDevice with pin {} and active_state "
                              "{}",
                              pin, active_state));
  massert(active(), "sanity");
}

FloatDevice::~FloatDevice() {}

bool FloatDevice::active() const {
  return device()->active();
}

float_sensor::status FloatDevice::read() const {
  massert(active(), "sanity");

  bool status = device()->read_bool();

  return status ? float_sensor::status::high : float_sensor::status::low;
}
}  // namespace device

NAMESPACE_END
