#ifndef LIB_MECHANISM_MOVEMENT_INLINE_HPP_
#define LIB_MECHANISM_MOVEMENT_INLINE_HPP_

#include "movement.hpp"

NAMESPACE_BEGIN

namespace mechanism {
template <movement::unit Unit>
void Movement::move(double x, double y, double z) {
  const long steps_x =
      convert_length_to_steps<Unit>(x, builder()->steps_per_mm_x());
  const long steps_y =
      convert_length_to_steps<Unit>(y, builder()->steps_per_mm_y());
  const long steps_z =
      convert_length_to_steps<Unit>(z, builder()->steps_per_mm_z());
  thread_pool().enqueue([this, steps_x, steps_y, steps_z] {
    LOG_DEBUG("Starting to move...");
    start_move(steps_x, steps_y, steps_z);
    while (!ready()) {
      // LOG_DEBUG("MOVE");
      next();
    }
    LOG_DEBUG("Move is finished");
  });
}
// template <movement::unit Unit>
// void Movement::move_x(double length) {
//   const long steps = convert_length_to_steps<Unit>(length, steps_per_mm());
//   auto       result = thread_pool().enqueue([this, steps] {
//     // LOG_DEBUG("START X!");
//     auto move_time = stepper_x()->time_for_move(steps);
//     LOG_DEBUG("Will move about {} micros", move_time);
//     // stepper_x()->move(steps);
//     // LOG_DEBUG("END X!");
//   });
// }

// template <movement::unit Unit>
// void Movement::move_y(double length) {
//   const long steps = convert_length_to_steps<Unit>(length, steps_per_mm());
//   thread_pool().enqueue([this, steps] {
//     // LOG_DEBUG("START Y!");
//     stepper_y()->move(steps);
//     // LOG_DEBUG("END Y!");
//   });
// }

// template <movement::unit Unit>
// void Movement::move_z(double length) {
//   const long steps = convert_length_to_steps<Unit>(length, steps_per_mm());
//   thread_pool().enqueue([this, steps] {
//     // LOG_DEBUG("START Z!");
//     stepper_z()->move(steps);
//     // LOG_DEBUG("END Z!");
//   });
// }
}  // namespace mechanism

NAMESPACE_END

#endif  // LIB_MECHANISM_MOVEMENT_INLINE_HPP_
