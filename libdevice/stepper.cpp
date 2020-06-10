#include "device.hpp"

#include "stepper.hpp"

#include <cmath>

NAMESPACE_BEGIN

namespace device {
const time_unit StepperDevice::step_high_min = 1;

StepperDevice::StepperDevice(PI_PIN        step_pin,
                             PI_PIN        dir_pin,
                             PI_PIN        enable_pin,
                             double        rpm,
                             stepper::step steps)
    : step_pin_{step_pin},
      dir_pin_{dir_pin},
      enable_pin_{enable_pin},
      rpm_{rpm},
      step_device_{DigitalOutputDevice::create(step_pin)},
      dir_device_{DigitalOutputDevice::create(dir_pin)},
      enable_device_{DigitalOutputDevice::create(enable_pin)},
      motor_steps_{steps} {
  DEBUG_ONLY(obj_name_ = "StepperDevice");
  /* Movement mechanism variables initialization */
  last_move_end_ = 0;
  next_move_interval_ = 0;
  acceleration_ = 1000;
  deceleration_ = 1000;
  direction_ = stepper::direction::forward;
  remaining_steps_ = 0;
  /*  End of movement mechanism variables initialization */
}

void StepperDevice::microsteps(const stepper::step& microsteps) {
  microsteps_ = microsteps;
}

void StepperDevice::motor_steps(const stepper::step& motor_steps) {
  motor_steps_ = motor_steps;
}

void StepperDevice::rpm(double rpm) {
  rpm_ = rpm;
}

void StepperDevice::acceleration(double acceleration) {
  acceleration_ = acceleration;
}

void StepperDevice::deceleration(double deceleration) {
  deceleration_ = deceleration;
}

void StepperDevice::enable() {
  step_device()->write(digital::value::high);
}

void StepperDevice::disable() {
  step_device()->write(digital::value::low);
}

namespace impl {
/** For constant speed */
template <>
void StepperDeviceImpl<stepper::speed::constant>::start_move(long      steps,
                                                             time_unit time) {
  if (steps == 0)
    return;

  pre_start_move(steps);
  steps_to_cruise_ = 0;
  steps_to_brake_ = 0;
  step_pulse_ = cruise_step_pulse_ =
      calc_step_pulse_from_rpm(motor_steps(), microsteps(), rpm());
  if (time > remaining_steps() * step_pulse()) {
    step_pulse_ = static_cast<unsigned long>(static_cast<float>(time) /
                                             remaining_steps());
  }
}

template <>
void StepperDeviceImpl<stepper::speed::constant>::calc_step_pulse() {
  // this should not be happening, but avoids strange calculations
  if (remaining_steps() <= 0) {
    return;
  }

  remaining_steps_--;
  step_count_++;
}

/** For linear speed */
template <>
void StepperDeviceImpl<stepper::speed::linear>::start_move(long      steps,
                                                           time_unit time) {
  pre_start_move(steps);

  // speed is in [steps/s]
  double speed = rpm() * motor_steps() / 60;

  if (time > 0) {
    // Calculate a new speed to finish in the time requested
    double t = static_cast<double>(time / (1e+6));  // convert to seconds
    double d = static_cast<double>(remaining_steps() /
                                   microsteps());  // convert to full steps
    double a2 = 1.0 / acceleration() + 1.0 / deceleration();
    double sqrt_candidate = t * t - 2.0 * a2 * d;  // in √b^2-4ac

    if (sqrt_candidate >= 0) {
      speed = std::min(
          speed, (t - static_cast<double>(std::sqrt(sqrt_candidate))) / a2);
    }
  }
  // how many microsteps from 0 to target speed
  steps_to_cruise_ = static_cast<stepper::step>(
      microsteps() * (speed * speed / (2 * acceleration())));
  // how many microsteps are needed from cruise speed to a full stop
  steps_to_brake_ = static_cast<stepper::step>(steps_to_cruise() *
                                               acceleration() / deceleration());
  if (remaining_steps() < steps_to_cruise() + steps_to_brake()) {
    // cannot reach max speed, will need to brake early
    steps_to_cruise_ = static_cast<stepper::step>(
        remaining_steps() * deceleration() / (acceleration() + deceleration()));

    steps_to_brake_ =
        static_cast<stepper::step>(remaining_steps() - steps_to_cruise());
  }
  // Initial pulse (c0) including error correction factor 0.676 [us]
  step_pulse_ = static_cast<stepper::pulse>(
      (1e+6) * 0.676 * std::sqrt(2.0f / acceleration() / microsteps()));
  // Save cruise timing since we will no longer have the calculated target speed
  // later
  cruise_step_pulse_ = static_cast<stepper::pulse>(1e+6 / speed / microsteps());
}

template <>
void StepperDeviceImpl<stepper::speed::linear>::calc_step_pulse() {
  // this should not be happening, but avoids strange calculations
  if (remaining_steps() <= 0) {
    return;
  }

  remaining_steps_--;
  step_count_++;

  switch (state()) {
    case stepper::state::accelerating:
      if (step_count() < steps_to_cruise()) {
        step_pulse_ = step_pulse() - (2 * step_pulse() + rest_steps()) /
                                         (4 * step_count() + 1);
        rest_steps_ =
            (step_count() < steps_to_cruise())
                ? (2 * step_pulse() + rest_steps()) % (4 * step_count() + 1)
                : 0;
      } else {
        // The series approximates target, set the final value to what it should
        // be instead
        step_pulse_ = cruise_step_pulse();
      }
      break;

    case stepper::state::decelerating:
      step_pulse_ = step_pulse() - (2 * step_pulse() + rest_steps()) /
                                       (-4 * remaining_steps() + 1);
      rest_steps_ =
          (2 * step_pulse() + rest_steps()) % (-4 * remaining_steps() + 1);
      break;

    default:
      break;  // no speed changes
  }
}

template <>
const time_unit StepperDeviceImpl<stepper::speed::constant>::time_for_move(
    long steps) {
  if (steps == 0) {
    return 0;
  }

  double t = std::abs(steps) *
             calc_step_pulse_from_rpm(motor_steps(), microsteps(), rpm());
  return static_cast<time_unit>(std::lround(t));
}

template <>
const time_unit StepperDeviceImpl<stepper::speed::linear>::time_for_move(
    long steps) {
  if (steps == 0) {
    return 0;
  }

  double t;

  start_move(steps);

  if (remaining_steps() >= steps_to_cruise() + steps_to_brake()) {
    double speed =
        static_cast<double>(rpm()) * motor_steps() / 60;  // full steps/s
    t = (static_cast<double>(std::abs(steps)) / (microsteps() * speed)) +
        (speed / (2 * acceleration())) +
        (speed / (2 * deceleration()));  // seconds
  } else {
    t = sqrt(2.0 * steps_to_cruise() / acceleration() /
             static_cast<double>(microsteps())) +
        sqrt(2.0 * steps_to_brake() / deceleration() /
             static_cast<double>(microsteps()));
  }

  t *= (1e+6);  // seconds -> micros

  return static_cast<time_unit>(std::lround(t));
}
}  // namespace impl
}  // namespace device

NAMESPACE_END
