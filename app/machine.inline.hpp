#ifndef APP_MACHINE_INLINE_HPP_
#define APP_MACHINE_INLINE_HPP_

#include "machine.hpp"

#include <libdevice/device.hpp>
#include <libgui/gui.hpp>
#include <libmechanism/mechanism.hpp>
#include <libutil/util.hpp>

NAMESPACE_BEGIN

namespace machine {
template <typename Event, typename FSM>
void TendingDef::initial::on_enter(Event const&&, FSM&) const {}

template <typename Event, typename FSM>
void TendingDef::running::no_task::on_enter(Event const&&, FSM& fsm) const {
  massert(State::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");
  massert(device::DigitalOutputDeviceRegistry::get() != nullptr, "sanity");

  auto*  state = State::get();
  auto&& movement = mechanism::movement_mechanism();
  auto*  digital_output_registry = device::DigitalOutputDeviceRegistry::get();

  auto&& spraying_ready =
      digital_output_registry->get(device::id::comm::pi::spraying_ready());
  auto&& tending_ready =
      digital_output_registry->get(device::id::comm::pi::tending_ready());

  spraying_ready->write(device::digital::value::low);
  state->spraying_ready(false);

  tending_ready->write(device::digital::value::low);
  state->tending_ready(false);

  LOG_INFO("Homing...");
  movement->homing();

  spraying_ready->write(device::digital::value::high);
  state->spraying_ready(true);

  tending_ready->write(device::digital::value::high);
  state->tending_ready(true);

  guard::spraying::height spraying_height;
  guard::tending::height  tending_height;

  root_machine(fsm).thread_pool().enqueue(
      [fsm, spraying_height, tending_height]() mutable -> void {
        while (!root_machine(fsm).is_terminated()) {
          if (spraying_height.check()) {
            root_machine(fsm).start_spraying();
            break;
          } else if (tending_height.check()) {
            root_machine(fsm).start_tending();
            break;
          }
          sleep_for<time_units::millis>(500);
        }
      });
}

/**
 * Beginning of spraying
 */
template <typename Event, typename FSM>
void TendingDef::running::spraying::idle::on_enter(Event const&&,
                                                   FSM& fsm) const {
  LOG_INFO("Entering spraying mode, waiting for signal for spraying height");
  fsm.initialize();
}

template <typename Event, typename FSM>
void TendingDef::running::spraying::preparation::on_enter(Event&&, FSM& fsm) {
  massert(State::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  auto*  state = State::get();
  auto&& movement = mechanism::movement_mechanism();

  LOG_INFO("Spraying preparation...");

  fsm.spraying_running->write(device::digital::value::low);
  state->spraying_running(false);

  fsm.spraying_complete->write(device::digital::value::low);
  state->spraying_complete(false);

  LOG_INFO("Homing to make sure ready to spray...");
  movement->homing();

  root_machine(fsm).run_spraying();
}

template <typename Event, typename FSM>
void TendingDef::running::spraying::preparation::on_exit(Event&&,
                                                         FSM const& fsm) const {
  massert(State::get() != nullptr, "sanity");

  auto* state = State::get();

  fsm.spraying_running->write(device::digital::value::low);
  state->spraying_running(false);

  fsm.spraying_complete->write(device::digital::value::low);
  state->spraying_complete(false);

  LOG_INFO("Spraying is ready, waiting for 3 seconds...");
  sleep_for<time_units::millis>(3000);
}

// template <typename FSM>
// void TendingDef::running::spraying::ongoing::on_enter(
//     event::spraying::run const&& event,
//     FSM&                         fsm) const {
//   LOG_INFO("Spraying...");
//   sleep_for<time_units::millis>(3000);
//   fsm.enclosing_fsm().is_spraying_completed_ = true;
// }

// template <typename Event, typename FSM>
// void TendingDef::running::spraying::ongoing::on_exit(Event const&& event,
//                                                      FSM const&    fsm) const
//                                                      {
//   LOG_INFO("Done spraying...");
// }
/**
 * End of spraying
 */

/**
 * Beginning of tending
 */
template <typename Event, typename FSM>
void TendingDef::running::tending::idle::on_enter(Event const&&,
                                                  FSM& fsm) const {
  LOG_INFO("Entering tending mode, waiting for signal for tending height");
  fsm.initialize();
}

template <typename Event, typename FSM>
void TendingDef::running::tending::preparation::on_enter(Event&&, FSM& fsm) {
  massert(State::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  auto*  state = State::get();
  auto&& movement = mechanism::movement_mechanism();

  LOG_INFO("Tending preparation...");

  fsm.tending_running->write(device::digital::value::low);
  state->tending_running(false);

  fsm.tending_complete->write(device::digital::value::low);
  state->tending_complete(false);

  LOG_INFO("Homing to make sure ready to tend...");
  movement->homing();

  LOG_INFO("Homing finger...");
  movement->homing_finger();

  root_machine(fsm).run_tending();
}

template <typename Event, typename FSM>
void TendingDef::running::tending::preparation::on_exit(Event&&,
                                                        FSM const& fsm) const {
  massert(State::get() != nullptr, "sanity");

  auto* state = State::get();

  fsm.tending_running->write(device::digital::value::low);
  state->tending_running(false);

  fsm.tending_complete->write(device::digital::value::low);
  state->tending_complete(false);

  LOG_INFO("Tending is ready, waiting for 3 seconds...");
  sleep_for<time_units::millis>(3000);
}
/**
 * End of tending
 */
}  // namespace machine

NAMESPACE_END

#endif  // APP_MACHINE_INLINE_HPP_
