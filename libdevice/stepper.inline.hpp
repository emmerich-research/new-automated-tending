#ifndef LIB_DEVICE_DIGITAL_STEPPER_STEPPER_INLINE_HPP_
#define LIB_DEVICE_DIGITAL_STEPPER_STEPPER_INLINE_HPP_

#include "stepper.hpp"

#include <chrono>
#include <cmath>
#include <thread>

NAMESPACE_BEGIN

namespace device {

namespace impl {
template <stepper::speed Speed>
StepperDeviceImpl<Speed>::StepperDeviceImpl(PI_PIN        step_pin,
                                            PI_PIN        dir_pin,
                                            PI_PIN        enable_pin,
                                            double        rpm,
                                            stepper::step steps)
    : StepperDevice{step_pin, dir_pin, enable_pin, rpm, steps} {
  /* Movement mechanism variables initialization */
  remaining_steps_ = 0;
  step_count_ = 0;
  steps_to_cruise_ = 0;
  steps_to_brake_ = 0;
  step_pulse_ = 0;
  cruise_step_pulse_ = 0;
  /*  End of movement mechanism variables initialization */
}

template <stepper::speed Speed>
stepper::state StepperDeviceImpl<Speed>::state() const {
  stepper::state state = stepper::state::stopped;

  if (remaining_steps() > 0) {
    if (remaining_steps() <= steps_to_brake()) {
      state = stepper::state::decelerating;
    } else if (step_count() <= steps_to_cruise()) {
      state = stepper::state::accelerating;
    } else {
      state = stepper::state::cruising;
    }
  } else {
    state = stepper::state::stopped;
  }

  return state;
}

template <stepper::speed Speed>
constexpr stepper::pulse StepperDeviceImpl<Speed>::calc_step_pulse_from_rpm(
    const stepper::step& steps,
    const stepper::step& microsteps,
    double               rpm) {
  return static_cast<stepper::pulse>(
      std::lround(60.0 * 1000000L / static_cast<double>(steps) /
                  static_cast<double>(microsteps) / rpm));
}

template <stepper::speed Speed>
void StepperDeviceImpl<Speed>::pre_start_move(long steps) {
  // set direction
  direction_ =
      (steps >= 0) ? stepper::direction::forward : stepper::direction::backward;

  // setup timer
  last_move_end_ = 0;
  // initialize steps
  remaining_steps_ = static_cast<stepper::step>(std::abs(steps));
  step_count_ = 0;
  rest_steps_ = 0;
}

template <stepper::speed Speed>
time_unit StepperDeviceImpl<Speed>::next(bool stop_condition) {
  if (stop_condition) {
    stop();
    return 0;
  }

  if (remaining_steps() > 0) {
    // original code : delayMicros(next_action_interval, last_action_end);

    // while (micros() - start_us < delay_us);
    while ((micros() - last_move_end()) < next_move_interval()) {
      // not yet running
      // return -1;
    }

    // DIR pin is sampled on rising STEP edge, so it is set first
    switch (direction()) {
      case stepper::direction::forward:
        dir_device()->write(digital::value::high);
        break;
      case stepper::direction::backward:
        dir_device()->write(digital::value::low);
        break;
    }

    // sleep_until<time_units::micros>(next_move_interval(), last_move_end());

    // start pulsing
    step_device()->write(digital::value::high);

    time_unit m = micros();
    // save value because calcStepPulse() will overwrite it
    unsigned long pulse = static_cast<unsigned long>(step_pulse());
    calc_step_pulse();

    // We should pull HIGH for at least 1-2us (step_high_min)
    sleep_for<time_units::micros>(StepperDevice::step_high_min);
    step_device()->write(digital::value::low);
    // end of pulsing

    // account for calcStepPulse() execution time;
    // sets ceiling for max rpm on slower MCUs
    last_move_end_ = micros();
    m = last_move_end() - m;
    next_move_interval_ = (pulse > m) ? pulse - m : 1;
  } else {
    // end of move
    last_move_end_ = 0;
    next_move_interval_ = 0;
  }

  return next_move_interval();
}

template <stepper::speed Speed>
void StepperDeviceImpl<Speed>::move(long steps, bool stop_condition) {
  start_move(steps);
  while (next(stop_condition) != 0) {
    // noop
    LOG_DEBUG("MOVE {}", remaining_steps());
  }
}

template <stepper::speed Speed>
stepper::step StepperDeviceImpl<Speed>::stop() {
  stepper::step retval = remaining_steps();
  remaining_steps_ = 0;
  return retval;
}

template <stepper::speed Speed>
double StepperDeviceImpl<Speed>::current_rpm() const {
  if (step_pulse() == 0) {
    return 0.0;
  }
  return static_cast<double>(60.0 * 1000000L / step_pulse() / microsteps() /
                             motor_steps());
}
}  // namespace impl
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_DIGITAL_STEPPER_STEPPER_INLINE_HPP_
