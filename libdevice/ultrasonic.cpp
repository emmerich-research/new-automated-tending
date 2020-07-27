#include "device.hpp"

#include "ultrasonic.hpp"

#include <libutil/util.hpp>

NAMESPACE_BEGIN

namespace device {
const double UltrasonicDevice::supersonic_speed = 1.0 / 29.1;  // in cm/us
const double UltrasonicDevice::max_distance = 400.0;           // in cm

UltrasonicDevice::UltrasonicDevice(PI_PIN echo_pin,
                                   PI_PIN trigger_pin,
                                   bool   echo_active_state,
                                   bool   trigger_active_state)
    : echo_pin_{echo_pin},
      trigger_pin_{trigger_pin},
      echo_device_{DigitalInputDevice::create(echo_pin, echo_active_state)},
      trigger_device_{
          DigitalOutputDevice::create(trigger_pin, trigger_active_state)} {
  DEBUG_ONLY_DEFINITION(
      obj_name_ = fmt::format("UltraSonicDevice echo_pin {} echo_active_state "
                              "{} trigger_pin {} trigger_active_state {}",
                              echo_pin, echo_active_state, trigger_pin,
                              trigger_active_state));
  massert(active(), "sanity");
}

UltrasonicDevice::~UltrasonicDevice() {}

double UltrasonicDevice::max_echo_time(double max_input_distance) const {
  // 10 is correction factor
  return (std::min(max_distance, max_input_distance) / (supersonic_speed * 2)) +
         10;
}

std::optional<double> UltrasonicDevice::distance(
    double max_input_distance) const {
  const double max_time = max_echo_time(max_input_distance);

  // trigger to low to make sure
  trigger_device()->write(digital::value::low);

  // wait for 5us
  sleep_for<time_units::micros>(5);

  // trigger to high
  trigger_device()->write(digital::value::high);

  // wait for 10us or 0.01ms
  sleep_for<time_units::micros>(10);

  // trigger to low
  trigger_device()->write(digital::value::low);

  time_unit start_task = micros();
  time_unit start = start_task;
  time_unit stop = start_task;

  while (!echo_device()->read_bool()) {
    start = micros();
    if ((start - start_task) > max_time) {
      return {};
    }
  }

  // update start_task
  start_task = micros();

  while (echo_device()->read_bool()) {
    stop = micros();
    if ((stop - start_task) > max_time) {
      return {};
    }
  }

  time_unit elapsed = stop - start;

  return (static_cast<double>(elapsed) / 2.0) * supersonic_speed;
}

bool UltrasonicDevice::active() const {
  return echo_device()->active() && trigger_device()->active();
}
}  // namespace device

NAMESPACE_END
