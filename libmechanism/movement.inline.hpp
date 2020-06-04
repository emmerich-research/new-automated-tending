#ifndef LIB_MECHANISM_MOVEMENT_INLINE_HPP_
#define LIB_MECHANISM_MOVEMENT_INLINE_HPP_

#include "movement.hpp"

NAMESPACE_BEGIN

namespace mechanism {
template <movement::unit Unit>
void Movement::move_x(double length) {
  LOG_DEBUG("CALLED!");
  const long steps = convert_length_to_steps<Unit>(length, steps_per_mm());
  auto       result = thread_pool().enqueue([this, steps] {
    // LOG_DEBUG("START X!");
    stepper_x()->move(steps);
    // LOG_DEBUG("END X!");
  });
}

template <movement::unit Unit>
void Movement::move_y(double length) {
  const long steps = convert_length_to_steps<Unit>(length, steps_per_mm());
  thread_pool().enqueue([this, steps] {
    // LOG_DEBUG("START Y!");
    stepper_y()->move(steps);
    // LOG_DEBUG("END Y!");
  });
}

template <movement::unit Unit>
void Movement::move_z(double length) {
  const long steps = convert_length_to_steps<Unit>(length, steps_per_mm());
  thread_pool().enqueue([this, steps] {
    // LOG_DEBUG("START Z!");
    stepper_z()->move(steps);
    // LOG_DEBUG("END Z!");
  });
}
}  // namespace mechanism

NAMESPACE_END

#endif  // LIB_MECHANISM_MOVEMENT_INLINE_HPP_
