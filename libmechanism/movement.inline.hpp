#ifndef LIB_MECHANISM_MOVEMENT_INLINE_HPP_
#define LIB_MECHANISM_MOVEMENT_INLINE_HPP_

#include "movement.hpp"

#include <cmath>

NAMESPACE_BEGIN

namespace mechanism {
template <movement::unit Unit>
long Movement::convert_length_to_steps(
    double                       length,
    const device::stepper::step& steps_per_mm) {
  if (Unit == movement::unit::cm) {
    return static_cast<long>(std::lround(length) * steps_per_mm * 10);
  } else {
    return static_cast<long>(std::lround(length) * steps_per_mm);
  }
}

template <movement::unit Unit>
void Movement::move(Point x, Point y, Point z) {
  if (ready()) {
    // enabling motor
    enable_motors();

    auto* state = State::get();

    auto current_x = state->x();
    auto current_y = state->y();
    auto current_z = state->z();

    LOG_INFO("Current X {}, Next X {}", current_x, x);
    LOG_INFO("Current Y {}, Next Y {}", current_y, y);
    LOG_INFO("Current Z {}, Next Z {}", current_z, z);

    long steps_x;
    long steps_y;
    long steps_z;

    if (state->manual_mode()) {
      steps_x = convert_length_to_steps<Unit>(x, builder()->steps_per_mm_x());
      steps_y = convert_length_to_steps<Unit>(y, builder()->steps_per_mm_y());
      steps_z = convert_length_to_steps<Unit>(z, builder()->steps_per_mm_z());

    } else {
      steps_x = convert_length_to_steps<Unit>(x - current_x,
                                              builder()->steps_per_mm_x());
      steps_y = convert_length_to_steps<Unit>(y - current_y,
                                              builder()->steps_per_mm_y());
      steps_z = convert_length_to_steps<Unit>(z - current_z,
                                              builder()->steps_per_mm_z());
    }

    LOG_INFO("Starting to move steps_x={}, steps_y={}, steps_z={}...", steps_x,
             steps_y, steps_z);
    start_move(steps_x, steps_y, steps_z);  // will trigger ready to false
    while (!ready()) {
      if (state->fault() && !state->manual_mode()) {
        stop();
        return;
      } else {
        next();
      }
    }
    LOG_INFO("Move is finished");

    if (state->manual_mode()) {
      state->coordinate({x + current_x, y + current_y, z + current_z});
    } else {
      state->coordinate({x, y, z});
    }

    // disabling motor
    disable_motors();
  }
}
}  // namespace mechanism

NAMESPACE_END

#endif  // LIB_MECHANISM_MOVEMENT_INLINE_HPP_
