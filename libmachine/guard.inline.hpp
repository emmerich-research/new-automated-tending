#ifndef LIB_MACHINE_GUARD_INLINE_HPP_
#define LIB_MACHINE_GUARD_INLINE_HPP_

#include "guard.hpp"

#include <afsm/fsm.hpp>

#include <libdevice/device.hpp>

NAMESPACE_BEGIN

namespace guard {
template <typename FSM, typename State>
bool machine_ready::operator()(FSM const& fsm, State const&) const {
  return fsm.is_ready();
}

template <typename FSM, typename State>
bool e_stop::operator()(FSM const&, State const&) const {
  return check();
}

template <typename FSM, typename State>
bool reset::operator()(FSM const&, State const&) const {
  return check();
}

template <typename FSM, typename State>
bool fault::operator()(FSM const&, State const&) const {
  return check();
}

namespace spraying {
template <typename FSM, typename State>
bool completed::operator()(FSM const&, State const&) const {
  return check();
}
}  // namespace spraying

namespace tending {
template <typename FSM, typename State>
bool completed::operator()(FSM const&, State const&) const {
  return check();
}
}  // namespace tending

namespace height {
template <typename FSM, typename State>
bool spraying_tending::operator()(FSM const&, State const&) const {
  return check();
}

template <typename FSM, typename State>
bool cleaning::operator()(FSM const&, State const&) const {
  return check();
}
}  // namespace height
}  // namespace height

NAMESPACE_END

#endif  // LIB_MACHINE_GUARD_INLINE_HPP_
