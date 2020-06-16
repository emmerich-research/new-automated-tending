#ifndef LIB_MECHANISM_MOVEMENT_INLINE_HPP_
#define LIB_MECHANISM_MOVEMENT_INLINE_HPP_

#include "movement.hpp"

NAMESPACE_BEGIN

namespace mechanism {
template <movement::unit Unit>
void Movement::move(double x, double y, double z) {
  if (ready()) {
    // enabling motor
    stepper_x()->enable();
    stepper_y()->enable();
    stepper_z()->enable();

    const long steps_x =
        convert_length_to_steps<Unit>(x, builder()->steps_per_mm_x());
    const long steps_y =
        convert_length_to_steps<Unit>(y, builder()->steps_per_mm_y());
    const long steps_z =
        convert_length_to_steps<Unit>(z, builder()->steps_per_mm_z());

    auto result = thread_pool().enqueue([this, steps_x, steps_y, steps_z] {
      LOG_DEBUG("Starting to move steps_x={}, steps_y={}, steps_z={}...",
                steps_x, steps_y, steps_z);
      start_move(steps_x, steps_y, steps_z);  // will trigger ready to false
      while (!ready()) {
        next();
      }
      LOG_DEBUG("Move is finished");
      return true;
    });

    // BEWARE!
    // this line will enforce waiting of move task until finished
    // this to simplify solution so we do not need any queue to implement with
    // if this line is deleted, then the move will be in race condition
    [[maybe_unused]] bool finished = result.get();

    // disabling motor
    stepper_x()->disable();
    stepper_y()->disable();
    stepper_z()->disable();
  }
}
}  // namespace mechanism

NAMESPACE_END

#endif  // LIB_MECHANISM_MOVEMENT_INLINE_HPP_
