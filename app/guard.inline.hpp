#ifndef APP_GUARD_INLINE_HPP_
#define APP_GUARD_INLINE_HPP_

#include "guard.hpp"

#include <libdevice/device.hpp>

NAMESPACE_BEGIN

namespace guard {
template <typename FSM, typename State>
bool homing::operator()(FSM const& fsm, State const&) const {
  return check();
}

namespace spraying {
template <typename FSM, typename State>
bool height::operator()(FSM const& fsm, State const&) const {
  return check();
}
}  // namespace spraying

namespace tending {
template <typename FSM, typename State>
bool height::operator()(FSM const& fsm, State const&) const {
  return check();
}
}  // namespace tending
}  // namespace guard

NAMESPACE_END

#endif  // APP_GUARD_INLINE_HPP_
