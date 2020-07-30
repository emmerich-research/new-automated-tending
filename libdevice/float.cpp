#include "device.hpp"

#include "float.hpp"

NAMESPACE_BEGIN

namespace device {

FloatDevice::FloatDevice(PI_PIN bottom_pin,
                         PI_PIN top_pin,
                         bool   bottom_active_state,
                         bool   top_active_state)
    : bottom_pin_{bottom_pin},
      top_pin_{top_pin},
      bottom_device_{
          DigitalInputDevice::create(bottom_pin, bottom_active_state)},
      top_device_{DigitalInputDevice::create(top_pin, top_active_state)} {
  DEBUG_ONLY_DEFINITION(
      obj_name_ = fmt::format(
          "FloatDevice with bottom_pin {} bottom_active_state "
          "{} top_pin {} top_active_state {}",
          bottom_pin, bottom_active_state, top_pin, top_active_state));
  massert(active(), "sanity");
}

FloatDevice::~FloatDevice() {}

bool FloatDevice::active() const {
  return bottom_device()->active() && top_device()->active();
}

float_sensor::status FloatDevice::read() const {
  massert(active(), "sanity");

  bool bottom = bottom_device()->read_bool();
  bool top = top_device()->read_bool();

  if (bottom && top) {
    return float_sensor::status::full;
  } else if (bottom && !top) {
    return float_sensor::status::normal;
  } else {
    return float_sensor::status::empty;
  }
}
}  // namespace device

NAMESPACE_END
