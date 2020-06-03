#ifndef LIB_DEVICE_DIGITAL_STEPPER_STEPPER_INLINE_HPP_
#define LIB_DEVICE_DIGITAL_STEPPER_STEPPER_INLINE_HPP_

#include "stepper/stepper.hpp"

#include <chrono>
#include <thread>

NAMESPACE_BEGIN

namespace device {
template <stepper::speed Speed>
const time_unit StepperDevice<Speed>::step_high_min = 1;

template <stepper::speed Speed>
StepperDevice<Speed>::StepperDevice(PI_PIN        step_pin,
                                    PI_PIN        dir_pin,
                                    PI_PIN        enable_pin,
                                    stepper::step steps)
    : step_pin_{step_pin},
      dir_pin_{dir_pin},
      enable_pin_{enable_pin},
      step_device_{DigitalOutputDevice::create(step_pin)},
      dir_device_{DigitalOutputDevice::create(dir_pin)},
      enable_device_{DigitalOutputDevice::create(enable_pin)},
      motor_steps_{steps} {
  /* Movement mechanism variables initialization */
  last_move_end_ = 0;
  next_move_interval_ = 0;
  acceleration_ = 1000;
  deceleration_ = 1000;
  direction_ = stepper::direction::forward;
  remaining_steps_ = 0;
  steps_to_cruise_ = 0;
  steps_to_brake_ = 0;
  step_pulse_ = 0;
  cruise_step_pulse_ = 0;
  /*  End of movement mechanism variables initialization */
}

template <stepper::speed Speed>
void StepperDevice<Speed>::microsteps(const stepper::step& microsteps) {
  microsteps_ = microsteps;
}

template <stepper::speed Speed>
void StepperDevice<Speed>::motor_steps(const stepper::step& motor_steps) {
  motor_steps_ = motor_steps;
}

template <stepper::speed Speed>
void StepperDevice<Speed>::rpm(double rpm) {
  rpm_ = rpm;
}

template <stepper::speed Speed>
void StepperDevice<Speed>::acceleration(double acceleration) {
  acceleration_ = acceleration;
}

template <stepper::speed Speed>
void StepperDevice<Speed>::deceleration(double deceleration) {
  deceleration_ = deceleration;
}

template <stepper::speed Speed>
void StepperDevice<Speed>::enable() {
  step_device()->write(digital::value::high);
}

template <stepper::speed Speed>
void StepperDevice<Speed>::disable() {
  step_device()->write(digital::value::low);
}

template <stepper::speed Speed>
const stepper::state StepperDevice<Speed>::state() const {
  stepper::state state = stepper::state::stopped;

  if (remaining_steps() > 0) {
    if (remaining_steps() <= steps_to_brake()) {
      state = stepper::state::decelerating;
    } else if (step_count() <= steps_to_cruise()) {
      state = stepper::state::accelerating;
    } else {
      state = stepper::state::cruising;
    }
  }

  return state;
}

template <stepper::speed Speed>
stepper::pulse StepperDevice<Speed>::calc_step_pulse_from_rpm(
    const stepper::step& steps,
    const stepper::step& microsteps,
    double               rpm) {
  return static_cast<stepper::pulse>(60.0 * 1000000L / steps / microsteps /
                                     rpm);
}

template <stepper::speed Speed>
void StepperDevice<Speed>::pre_start_move(long steps) {
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
bool StepperDevice<Speed>::yield_move(void) {
  if (remaining_steps() > 0) {
    // original code :delayMicros(next_action_interval, last_action_end);
    sleep_until<time_units::micros>(next_move_interval(), last_move_end());

    // DIR pin is sampled on rising STEP edge, so it is set first
    switch (direction()) {
      case stepper::direction::forward:
        dir_device()->write(digital::value::high);
        break;
      case stepper::direction::backward:
        dir_device()->write(digital::value::low);
        break;
      default:
        // noop
        break;
    }

    // start pulsing
    step_device()->write(digital::value::high);

    time_unit m = micros();
    // save value because calcStepPulse() will overwrite it
    stepper::pulse pulse = step_pulse();
    calc_step_pulse();

    // We should pull HIGH for at least 1-2us (step_high_min)
    sleep_for<time_units::micros>(step_high_min);
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

  return next_move_interval() > 0;
}

template <stepper::speed Speed>
void StepperDevice<Speed>::move(long steps) {
  start_move(steps);
  while (yield_move()) {
    // noop
  }
}
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_DIGITAL_STEPPER_STEPPER_INLINE_HPP_
