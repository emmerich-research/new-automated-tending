#ifndef APP_MACHINE_INLINE_HPP_
#define APP_MACHINE_INLINE_HPP_

#include "machine.hpp"

#include <libmechanism/mechanism.hpp>
#include <libutil/util.hpp>

NAMESPACE_BEGIN

namespace machine {
template <typename FSM>
void TendingDef::spraying::idle::on_enter(event::spraying::start&& event,
                                          FSM&                     fsm) const {
  LOG_INFO("Entering spraying mode, waiting for signal for spraying height");
  // auto*  output_registry = device::DigitalOutputDeviceRegistry::get();
  // auto&& spraying_ready =
  //     output_registry->get(device::id::comm::pi::spraying_ready());

  // low for not ready, high for ready
  // always write not ready

  // if (spraying_last_value_) {
  // spraying_ready->write(device::digital::value::low);
  // } else {
  // }
}

template <typename Event, typename FSM>
void TendingDef::spraying::preparation::on_enter(Event&& event,
                                                 FSM&    fsm) const {
  // massert(mechanism::movement_mechanism() != nullptr, "sanity");
  // massert(mechanism::movement_mechanism()->active(), "sanity");

  // auto* movement = mechanism::movement_mechanism();
  // movement->homing();
}

template <typename Event, typename FSM>
void TendingDef::spraying::preparation::on_exit(Event&& event, FSM& fsm) const {
  massert(device::DigitalOutputDeviceRegistry::get() != nullptr, "sanity");
  auto*  output_registry = device::DigitalOutputDeviceRegistry::get();
  auto&& spraying_ready =
      output_registry->get(device::id::comm::pi::spraying_ready());

  spraying_ready->write(device::digital::value::high);
  LOG_INFO("Spraying is ready, waiting for 3 seconds...");
  sleep_for<time_units::millis>(3000);
  fsm.spraying_ready_last_value_ = true;
}
}  // namespace machine

NAMESPACE_END

#endif  // APP_MACHINE_INLINE_HPP_
