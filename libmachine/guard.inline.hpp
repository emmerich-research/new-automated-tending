#ifndef APP_GUARD_INLINE_HPP_
#define APP_GUARD_INLINE_HPP_

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

namespace spraying {
template <typename FSM, typename State>
bool height::operator()(FSM const&, State const&) const {
  return check();
}

template <typename FSM, typename State>
auto completed::operator()(FSM const& fsm, State const&) const
    -> decltype(fsm.is_spraying_completed(), bool()) {
  return fsm.is_spraying_completed();
}

template <typename FSM, typename State>
auto completed::operator()(FSM const& fsm, State const&) const
    -> decltype(fsm.enclosing_fsm().is_spraying_completed(), bool()) {
  return fsm.enclosing_fsm().is_spraying_completed();
}
}  // namespace spraying

namespace tending {
template <typename FSM, typename State>
bool height::operator()(FSM const&, State const&) const {
  return check();
}

template <typename FSM, typename State>
auto completed::operator()(FSM const& fsm, State const&) const
    -> decltype(fsm.is_tending_completed(), bool()) {
  return fsm.is_tending_completed();
}

template <typename FSM, typename State>
auto completed::operator()(FSM const& fsm, State const&) const
    -> decltype(fsm.enclosing_fsm().is_tending_completed(), bool()) {
  return fsm.enclosing_fsm().is_tending_completed();
}
}  // namespace tending
}  // namespace guard

NAMESPACE_END

#endif  // APP_GUARD_INLINE_HPP_
