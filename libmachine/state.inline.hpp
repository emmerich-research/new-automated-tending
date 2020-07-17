#ifndef LIB_MACHINE_STATE_INLINE_HPP_
#define LIB_MACHINE_STATE_INLINE_HPP_

#include "state.hpp"

#include <libdevice/device.hpp>
#include <libmechanism/mechanism.hpp>
#include <libutil/util.hpp>

#include "util.hpp"

NAMESPACE_BEGIN

namespace machine {
template <typename Event, typename FSM>
void TendingDef::initial::on_enter(Event const&&, FSM&) const {}

template <typename Event, typename FSM>
void TendingDef::running::no_task::on_enter(Event const&&, FSM& fsm) const {
  massert(State::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");

  root_machine(fsm).thread_pool().enqueue([fsm]() mutable -> void {
    auto*  state = State::get();
    auto*  shift_register = device::ShiftRegister::get();
    auto&& movement = mechanism::movement_mechanism();

    // state->spraying_fault(false);
    // state->tending_fault(false);
    state->fault(false);
    state->manual_mode(false);

    shift_register->write(device::id::comm::pi::spraying_ready(),
                          device::digital::value::low);
    state->spraying_ready(false);

    shift_register->write(device::id::comm::pi::tending_ready(),
                          device::digital::value::low);
    state->tending_ready(false);

    LOG_INFO("Homing...");
    movement->homing();

    shift_register->write(device::id::comm::pi::spraying_ready(),
                          device::digital::value::high);
    state->spraying_ready(true);

    shift_register->write(device::id::comm::pi::tending_ready(),
                          device::digital::value::high);
    state->tending_ready(true);

    guard::spraying::height spraying_height;
    guard::tending::height  tending_height;

    while (!root_machine(fsm).is_terminated() && !state->fault()) {
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
}

template <typename Event, typename FSM>
void TendingDef::running::spraying::preparation::on_enter(Event&&, FSM& fsm) {
  massert(State::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");

  auto*  state = State::get();
  auto*  shift_register = device::ShiftRegister::get();
  auto&& movement = mechanism::movement_mechanism();

  LOG_INFO("Spraying preparation...");

  shift_register->write(device::id::comm::pi::spraying_running(),
                        device::digital::value::low);
  state->spraying_running(false);

  shift_register->write(device::id::comm::pi::spraying_complete(),
                        device::digital::value::low);
  state->spraying_complete(false);

  LOG_INFO("Homing to make sure ready to spray...");
  movement->homing();

  root_machine(fsm).run_spraying();
}

template <typename Event, typename FSM>
void TendingDef::running::spraying::preparation::on_exit(Event&&,
                                                         FSM const& fsm) const {
  massert(State::get() != nullptr, "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");

  auto* state = State::get();
  auto* shift_register = device::ShiftRegister::get();

  shift_register->write(device::id::comm::pi::tending_running(),
                        device::digital::value::low);
  state->spraying_running(false);

  shift_register->write(device::id::comm::pi::tending_complete(),
                        device::digital::value::low);
  state->spraying_complete(false);

  LOG_INFO("Spraying is ready, waiting for 3 seconds...");
  sleep_for<time_units::millis>(3000);
}
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
}

template <typename Event, typename FSM>
void TendingDef::running::tending::preparation::on_enter(Event&&, FSM& fsm) {
  massert(State::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");

  auto*  state = State::get();
  auto*  shift_register = device::ShiftRegister::get();
  auto&& movement = mechanism::movement_mechanism();

  LOG_INFO("Tending preparation...");

  shift_register->write(device::id::comm::pi::tending_running(),
                        device::digital::value::low);
  state->tending_running(false);

  shift_register->write(device::id::comm::pi::tending_complete(),
                        device::digital::value::low);
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
  auto* shift_register = device::ShiftRegister::get();

  shift_register->write(device::id::comm::pi::tending_running(),
                        device::digital::value::low);
  state->tending_running(false);

  shift_register->write(device::id::comm::pi::tending_complete(),
                        device::digital::value::low);
  state->tending_complete(false);

  LOG_INFO("Tending is ready, waiting for 3 seconds...");
  sleep_for<time_units::millis>(3000);
}
/**
 * End of tending
 */

/**
 * Beginning of Stopped
 */
template <typename Event, typename FSM>
void TendingDef::fault::idle::on_enter(Event const&&, FSM& fsm) const {
  massert(State::get() != nullptr, "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");

  auto* state = State::get();
  auto* shift_register = device::ShiftRegister::get();

  LOG_INFO("Entering fault mode");
  machine::util::reset_task_state();
}

template <typename Event, typename FSM>
void TendingDef::fault::manual::on_enter(Event const&&, FSM& fsm) const {
  massert(State::get() != nullptr, "sanity");
  LOG_INFO("Entering fault manual mode");

  auto* state = State::get();

  state->manual_mode(true);
}

template <typename Event, typename FSM>
void TendingDef::fault::manual::on_exit(Event const&&, FSM& fsm) const {
  massert(State::get() != nullptr, "sanity");
  LOG_INFO("Exiting fault manual mode");

  auto* state = State::get();

  state->manual_mode(false);
}
}  // namespace machine

NAMESPACE_END

#endif  // LIB_MACHINE_STATE_INLINE_HPP_
