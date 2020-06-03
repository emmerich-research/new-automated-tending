#include "device.hpp"

#include "stepper/stepper.hpp"

#include <cmath>

NAMESPACE_BEGIN

namespace device {
/** For constant speed */
template <>
void StepperDevice<stepper::speed::constant>::start_move(long      steps,
                                                         time_unit time) {
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
void StepperDevice<stepper::speed::constant>::calc_step_pulse() {
  // this should not be happening, but avoids strange calculations
  if (remaining_steps() <= 0) {
    return;
  }

  remaining_steps_--;
  step_count_++;
}

/** For linear speed */
template <>
void StepperDevice<stepper::speed::linear>::start_move(long      steps,
                                                       time_unit time) {
  pre_start_move(steps);

  // speed is in [steps/s]
  double speed = rpm() * motor_steps() / 60;

  if (time > 0) {
    // Calculate a new speed to finish in the time requested
    double t = static_cast<double>(time / (1e+6));  // convert to seconds
    double d = static_cast<double>(remaining_steps() /
                                   microsteps());  // convert to full steps
    double a2 =
        static_cast<double>(1.0 / acceleration() + 1.0 / deceleration());
    double sqrt_candidate =
        static_cast<double>(t * t - 2 * a2 * d);  // in √b^2-4ac

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
void StepperDevice<stepper::speed::linear>::calc_step_pulse() {
  // this should not be happening, but avoids strange calculations
  if (remaining_steps() <= 0){
    return;
  }

  remaining_steps_--;
  step_count_++;

  switch (state()) {
    case stepper::state::accelerating:
      if (step_count() < steps_to_cruise()){
        step_pulse_ = step_pulse() - (2 * step_pulse() + rest_steps()) / (4 * step_count() + 1);
        rest_steps_ = (step_count() < steps_to_cruise()) ? (2 * step_pulse() + rest_steps()) % (4 * step_count() + 1) : 0;
      } else {
        // The series approximates target, set the final value to what it should be instead
        step_pulse_ = cruise_step_pulse();
      }
      break;

    case stepper::state::decelerating:
      step_pulse_ = step_pulse() - (2 * step_pulse() + rest_steps()) / (-4 * remaining_steps() + 1);
      rest_steps_ = (2 * step_pulse() + rest_steps()) % (-4 * remaining_steps() + 1);
      break;

    default:
      break; // no speed changes
  }
}
}  // namespace device

NAMESPACE_END
