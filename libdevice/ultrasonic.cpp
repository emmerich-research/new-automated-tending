#include "device.hpp"

#include "ultrasonic.hpp"

#include <libutil/util.hpp>

NAMESPACE_BEGIN

namespace device {
UltrasonicDevice::UltrasonicDevice(PI_PIN echo_pin,
                                   PI_PIN trigger_pin,
                                   bool   echo_active_state,
                                   bool   trigger_active_state)
    : echo_pin_{echo_pin},
      trigger_pin_{trigger_pin},
      echo_device_{DigitalInputDevice::create(echo_pin, echo_active_state)},
      trigger_device_{
          DigitalOutputDevice::create(trigger_pin, trigger_active_state)} {
  DEBUG_ONLY(obj_name_ =
                 fmt::format("UltraSonicDevice echo_pin {} echo_active_state "
                             "{} trigger_pin {} trigger_active_state {}",
                             echo_pin, echo_active_state, trigger_pin,
                             trigger_active_state));
  massert(active(), "sanity");
}

UltrasonicDevice::~UltrasonicDevice() {}

double UltrasonicDevice::distance() const {
  // trigger to high
  trigger_device()->write(digital::value::high);

  // sleep for 10us or 0.01ms
  sleep_for<time_units::micros>(10);

  // trigger to low
  trigger_device()->write(digital::value::low);

  time_unit start = micros();
  time_unit stop = micros();

  while (!echo_device()->read_bool()) {
    start = micros();
  }

  while (echo_device()->read_bool()) {
    stop = micros();
  }

  time_unit elapsed = stop - start;

  // supersonic speed = 0.03432 cm/us = 34320 cm/s == 343.2 m/s
  return static_cast<double>(elapsed) * 0.03432 / 2;
}

bool UltrasonicDevice::active() const {
  return echo_device()->active() && trigger_device()->active();
}
}  // namespace device

NAMESPACE_END
