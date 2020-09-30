#ifndef LIB_MACHINE_ACTION_INLINE_HPP_
#define LIB_MACHINE_ACTION_INLINE_HPP_

#include <libcore/core.hpp>
#include <libdevice/device.hpp>
#include <libgui/gui.hpp>
#include <libmechanism/mechanism.hpp>
#include <libutil/util.hpp>

#include "action.hpp"

#include "util.hpp"

NAMESPACE_BEGIN

namespace action {
template <typename Event,
          typename FSM,
          typename SourceState,
          typename TargetState>
void start::operator()(Event const&,
                       FSM& fsm,
                       SourceState&,
                       TargetState&) const {
  ATM_STATUS status = ATM_OK;

  // initialize `GPIO-based` devices such as analog, digital, and PWM
  status = initialize_device();
  if (status == ATM_ERR) {
    throw std::runtime_error(
        "Failed to initialize `device`, something is wrong");
  }

  // initialize `mechanism`
  status = initialize_mechanism();
  if (status == ATM_ERR) {
    throw std::runtime_error(
        "Failed to initialize `mechanism`, something is wrong");
  }

  // setting all communication to low
  auto* shift_register = device::ShiftRegister::get();
  massert(shift_register != nullptr, "sanity");

  machine::util::reset_task_state();

  fsm.machine_ready_ = true;
}

template <typename Event,
          typename FSM,
          typename SourceState,
          typename TargetState>
void stop::operator()(Event const&, FSM&, SourceState&, TargetState&) const {
  shutdown_hook();
}

template <typename Event,
          typename FSM,
          typename SourceState,
          typename TargetState>
void fault::operator()(Event const&, FSM&, SourceState&, TargetState&) const {}

template <typename Event,
          typename FSM,
          typename SourceState,
          typename TargetState>
void restart::operator()(Event const&, FSM&, SourceState&, TargetState&) const {
  massert(State::get() != nullptr, "sanity");

  auto* state = State::get();

  state->homing(false);
  state->fault(false);
}

namespace spraying {
template <typename Event,
          typename FSM,
          typename SourceState,
          typename TargetState>
void job::operator()(Event const&, FSM& fsm, SourceState&, TargetState&) const {
  massert(State::get() != nullptr, "sanity");
  massert(device::DigitalOutputDeviceRegistry::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");

  auto*  state = State::get();
  auto*  shift_register = device::ShiftRegister::get();
  auto&& movement = mechanism::movement_mechanism();

  if (state->fault())
    return;

  LOG_INFO("Spraying...");
  shift_register->write(device::id::comm::pi::spraying_running(),
                        device::digital::value::high);
  state->spraying_running(true);

  if (state->fault())
    return;

  LOG_INFO("Moving to spraying position...");
  movement->move_to_spraying_position();

  if (state->fault())
    return;

  sleep_for<time_units::millis>(3000);

  LOG_INFO("Turning on the spray...");
  shift_register->write(device::id::spray(), device::digital::value::high);

  if (state->fault())
    return;

  sleep_for<time_units::millis>(3000);

  if (state->fault())
    return;

  LOG_INFO("Follow spraying paths...");
  movement->follow_spraying_paths();

  if (state->fault())
    return;

  LOG_INFO("Turning off the spray...");
  shift_register->write(device::id::spray(), device::digital::value::low);

  if (state->fault())
    return;

  LOG_INFO("Homing...");
  movement->homing();

  if (state->fault())
    return;

  shift_register->write(device::id::comm::pi::spraying_running(),
                        device::digital::value::low);
  state->spraying_running(false);

  shift_register->write(device::id::comm::pi::spraying_complete(),
                        device::digital::value::high);
  state->spraying_complete(true);
}

template <typename Event,
          typename FSM,
          typename SourceState,
          typename TargetState>
void complete::operator()(Event const&, FSM& fsm, SourceState&, TargetState&) {
  massert(State::get() != nullptr, "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");

  auto* state = State::get();
  auto* shift_register = device::ShiftRegister::get();

  LOG_INFO("Spraying is completed...");

  sleep_for<time_units::millis>(3000);
  shift_register->write(device::id::comm::pi::spraying_complete(),
                        device::digital::value::low);
  state->spraying_complete(false);

  // reset back the spraying, tending, and cleaning ready to true
  machine::util::reset_task_ready();

  sleep_for<time_units::millis>(1000);
  root_machine(fsm).task_completed();
}
}  // namespace spraying

namespace tending {
template <typename Event,
          typename FSM,
          typename SourceState,
          typename TargetState>
void job::operator()(Event const&, FSM& fsm, SourceState&, TargetState&) const {
  massert(Config::get() != nullptr, "sanity");
  massert(State::get() != nullptr, "sanity");
  massert(device::DigitalOutputDeviceRegistry::get() != nullptr, "sanity");
  massert(device::PWMDeviceRegistry::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");

  auto*        config = Config::get();
  auto*        pwm_registry = device::PWMDeviceRegistry::get();
  auto*        state = State::get();
  auto*        shift_register = device::ShiftRegister::get();
  const auto&& movement = mechanism::movement_mechanism();
  auto&&       finger = pwm_registry->get(device::id::finger());

  if (state->fault())
    return;

  LOG_INFO("Tending begins...");
  shift_register->write(device::id::comm::pi::tending_running(),
                        device::digital::value::high);
  state->tending_running(true);

  if (state->fault())
    return;

  LOG_INFO("Moving to tending position...");
  movement->move_to_tending_position();

  if (state->fault())
    return;

  sleep_for<time_units::millis>(3000);

  if (state->fault())
    return;

  LOG_INFO("Moving finger down...");
  movement->move_finger_down();

  if (state->fault())
    return;

  sleep_for<time_units::millis>(1000);

  if (state->fault())
    return;

  LOG_INFO("Following edge paths...");
  movement->follow_tending_paths_edge();

  if (state->fault())
    return;

  sleep_for<time_units::millis>(1000);

  if (state->fault())
    return;

  LOG_INFO("Turning on the motor...");
  movement->rotate_finger();

  if (state->fault())
    return;

  sleep_for<time_units::millis>(1000);

  if (state->fault())
    return;

  LOG_INFO("Follow zigzag paths...");
  movement->follow_tending_paths_zigzag();

  if (state->fault())
    return;

  sleep_for<time_units::millis>(1000);

  if (state->fault())
    return;

  LOG_INFO("Stop finger...");
  movement->stop_finger();

  if (state->fault())
    return;

  LOG_INFO("Homing...");
  movement->homing();

  if (state->fault())
    return;

  shift_register->write(device::id::comm::pi::tending_running(),
                        device::digital::value::low);
  state->tending_running(false);

  shift_register->write(device::id::comm::pi::tending_complete(),
                        device::digital::value::high);
  state->tending_complete(true);
}

template <typename Event,
          typename FSM,
          typename SourceState,
          typename TargetState>
void complete::operator()(Event const&, FSM& fsm, SourceState&, TargetState&) {
  massert(State::get() != nullptr, "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");

  auto* state = State::get();
  auto* shift_register = device::ShiftRegister::get();

  LOG_INFO("Tending is completed...");

  sleep_for<time_units::millis>(3000);
  shift_register->write(device::id::comm::pi::tending_complete(),
                        device::digital::value::low);
  // state->tending_complete(false);

  // reset back the spraying, tending, and cleaning ready to true
  machine::util::reset_task_ready();

  sleep_for<time_units::millis>(1000);
  root_machine(fsm).task_completed();
}
}  // namespace tending

namespace cleaning {
template <typename Event,
          typename FSM,
          typename SourceState,
          typename TargetState>
void job::operator()(Event const&, FSM& fsm, SourceState&, TargetState&) const {
  massert(Config::get() != nullptr, "sanity");
  massert(State::get() != nullptr, "sanity");
  massert(device::DigitalOutputDeviceRegistry::get() != nullptr, "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  auto*  config = Config::get();
  auto*  state = State::get();
  auto*  digital_output_registry = device::DigitalOutputDeviceRegistry::get();
  auto*  shift_register = device::ShiftRegister::get();
  auto&& movement = mechanism::movement_mechanism();

  auto&& sonicator_relay =
      digital_output_registry->get(device::id::sonicator_relay());

  if (state->fault())
    return;

  LOG_INFO("Cleaning begins...");
  state->cleaning_running(true);

  LOG_INFO("Homing finger...");
  movement->homing_finger();

  if (state->fault())
    return;

  for (const auto& [x, y, time, sonicator] : config->cleaning_stations()) {
    if (state->fault())
      return;

    LOG_INFO("Moving to cleaning station with x:{} y:{}", x, y);
    movement->move<mechanism::movement::unit::mm>(x, y, 0.0);

    if (state->fault())
      return;

    LOG_INFO("Moving finger down");
    movement->move_finger_down();

    if (state->fault())
      return;

    if (sonicator) {
      LOG_INFO("Turning on the sonicator relay");
      sonicator_relay->write(device::digital::value::high);
      /*shift_register->write(device::id::comm::pi::sonicator_relay(),*/
      /*device::digital::value::high);*/
    }

    if (state->fault())
      return;

    LOG_INFO("Wait for {} seconds", time);
    sleep_for<time_units::seconds>(time);

    if (state->fault())
      return;

    if (sonicator) {
      LOG_INFO("Turning off the sonicator relay");
      sonicator_relay->write(device::digital::value::low);
      /*shift_register->write(device::id::comm::pi::sonicator_relay(),*/
      /*device::digital::value::low);*/
    }

    if (state->fault())
      return;

    LOG_INFO("Moving finger up");
    movement->move_finger_up();

    if (state->fault())
      return;
  }

  if (state->fault())
    return;

  state->cleaning_running(false);
  state->cleaning_complete(true);
}

template <typename Event,
          typename FSM,
          typename SourceState,
          typename TargetState>
void complete::operator()(Event const&, FSM& fsm, SourceState&, TargetState&) {
  massert(State::get() != nullptr, "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  auto* state = State::get();
  auto* shift_register = device::ShiftRegister::get();

  LOG_INFO("Cleaning is completed...");

  sleep_for<time_units::millis>(3000);

  // add teding complete to make cleaning is done only once
  state->tending_complete(false);
  state->cleaning_complete(false);

  // reset back the spraying, tending, and cleaning ready to true
  machine::util::reset_task_ready();

  sleep_for<time_units::millis>(1000);
  root_machine(fsm).task_completed();
}
}  // namespace cleaning
}  // namespace action

NAMESPACE_END

#endif  // LIB_MACHINE_ACTION_INLINE_HPP_
