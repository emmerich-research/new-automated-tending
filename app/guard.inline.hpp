#ifndef APP_GUARD_INLINE_HPP_
#define APP_GUARD_INLINE_HPP_

#include "guard.hpp"

#include <libdevice/device.hpp>

NAMESPACE_BEGIN

namespace guard {
template <typename FSM, typename State>
bool spraying_height::operator()(FSM const& fsm, State const&) const {
  auto*  input_registry = device::DigitalInputDeviceRegistry::get();
  auto&& spraying_height =
      input_registry->get(device::id::comm::plc::spraying_height());
  return spraying_height->read_bool();
}

template <typename FSM, typename State>
bool tending_height::operator()(FSM const& fsm, State const&) const {
  auto*  input_registry = device::DigitalInputDeviceRegistry::get();
  auto&& tending_height =
      input_registry->get(device::id::comm::plc::tending_height());
  return tending_height->read_bool();
}
}  // namespace guard

NAMESPACE_END

#endif  // APP_GUARD_INLINE_HPP_
