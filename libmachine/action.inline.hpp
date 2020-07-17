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
  // std::cout << "Starting machine ..." << std::endl;

  // preparing shutdown hook
  // std::atexit(shutdown_hook);

  ATM_STATUS status = ATM_OK;

  // initialize `core` such as config, logger, and state
  status = initialize_core();
  if (status == ATM_ERR) {
    throw std::runtime_error(
        "Failed to initialize `core` functionality, something is wrong");
  }

  // status = initialize_gui();
  // if (status == ATM_ERR) {
  //   throw std::runtime_error(
  //       "Failed to initialize `gui` functionality, something is wrong");
  // }

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

  // shift_register->write(device::id::comm::pi::spraying_ready(),
  //                       device::digital::value::low);
  // shift_register->write(device::id::comm::pi::spraying_running(),
  //                       device::digital::value::low);
  // shift_register->write(device::id::comm::pi::spraying_complete(),
  //                       device::digital::value::low);

  // shift_register->write(device::id::comm::pi::tending_ready(),
  //                       device::digital::value::low);
  // shift_register->write(device::id::comm::pi::tending_running(),
  //                       device::digital::value::low);
  // shift_register->write(device::id::comm::pi::tending_complete(),
  //                       device::digital::value::low);

  // setting global state
  // auto* state = State::get();
  // massert(state != nullptr, "sanity");
  // state->spraying_ready(false);
  // state->spraying_running(false);
  // state->spraying_complete(false);
  // // state->spraying_fault(false);

  // state->tending_ready(false);
  // state->tending_running(false);
  // state->tending_complete(false);
  // state->tending_fault(false);

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
void fault::operator()(Event const&, FSM&, SourceState&, TargetState&) const {
  massert(State::get() != nullptr, "sanity");

  auto* state = State::get();

  // if (state->spraying_running()) {
  //   state->spraying_fault(true);
  // } else if (state->tending_running()) {
  //   state->tending_fault(true);
  // }

  state->fault(true);
}

template <typename Event,
          typename FSM,
          typename SourceState,
          typename TargetState>
void restart::operator()(Event const&, FSM&, SourceState&, TargetState&) const {
  massert(State::get() != nullptr, "sanity");

  auto* state = State::get();

  state->fault(false);

  // if (state->spraying_running()) {
  //   state->spraying_fault(false);
  // } else if (state->tending_running()) {
  //   state->tending_fault(false);
  // }
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

  LOG_INFO("Spraying...");
  shift_register->write(device::id::comm::pi::spraying_running(),
                        device::digital::value::high);
  // fsm.spraying_running->write(device::digital::value::high);
  state->spraying_running(true);

  LOG_INFO("Moving to spraying position...");
  movement->move_to_spraying_position();

  sleep_for<time_units::millis>(3000);

  LOG_INFO("Turning on the spray...");
  shift_register->write(device::id::spray(), device::digital::value::high);
  // fsm.spray->write(device::digital::value::high);

  sleep_for<time_units::millis>(3000);

  LOG_INFO("Follow spraying paths...");
  movement->follow_spraying_paths();

  LOG_INFO("Turning off the spray...");
  shift_register->write(device::id::spray(), device::digital::value::low);
  // fsm.spray->write(device::digital::value::low);

  LOG_INFO("Homing...");
  movement->homing();

  // this needed for transition
  fsm.enclosing_fsm().is_spraying_completed_ = true;
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

  shift_register->write(device::id::comm::pi::spraying_running(),
                        device::digital::value::low);
  state->spraying_running(false);

  shift_register->write(device::id::comm::pi::spraying_complete(),
                        device::digital::value::high);
  state->spraying_complete(true);

  LOG_INFO("Spraying is completed...");

  sleep_for<time_units::millis>(3000);
  shift_register->write(device::id::comm::pi::spraying_complete(),
                        device::digital::value::low);
  state->spraying_complete(false);

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

  LOG_INFO("Tending begins...");
  shift_register->write(device::id::comm::pi::tending_running(),
                        device::digital::value::high);
  state->tending_running(true);

  LOG_INFO("Moving to tending position...");
  movement->move_to_tending_position();

  sleep_for<time_units::millis>(3000);

  LOG_INFO("Moving finger down...");
  movement->move_finger_down();

  sleep_for<time_units::millis>(1000);

  LOG_INFO("Following edge paths...");
  movement->follow_tending_paths_edge();

  sleep_for<time_units::millis>(1000);

  LOG_INFO("Turning on the motor...");

  if (finger->duty_cycle(config->finger<unsigned int>("duty-cycle")) ==
      ATM_ERR) {
    LOG_INFO("Cannot set finger duty cycle...");
  }

  sleep_for<time_units::millis>(1000);

  LOG_INFO("Follow zigzag paths...");
  movement->follow_tending_paths_zigzag();

  sleep_for<time_units::millis>(1000);

  LOG_INFO("Homing finger...");
  movement->homing_finger();

  LOG_INFO("Homing...");
  movement->homing();

  // this needed for transition
  fsm.enclosing_fsm().is_tending_completed_ = true;
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

  shift_register->write(device::id::comm::pi::tending_running(),
                        device::digital::value::low);
  state->tending_running(false);

  shift_register->write(device::id::comm::pi::tending_complete(),
                        device::digital::value::high);
  state->tending_complete(true);

  LOG_INFO("Tending is completed...");

  sleep_for<time_units::millis>(3000);
  shift_register->write(device::id::comm::pi::tending_complete(),
                        device::digital::value::low);
  state->tending_complete(false);

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
}

template <typename Event,
          typename FSM,
          typename SourceState,
          typename TargetState>
void complete::operator()(Event const&, FSM& fsm, SourceState&, TargetState&) {}
}  // namespace cleaning

NAMESPACE_END

#endif  // LIB_MACHINE_ACTION_INLINE_HPP_
