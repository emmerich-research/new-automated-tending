#ifndef APP_ACTION_INLINE_HPP_
#define APP_ACTION_INLINE_HPP_

#include <libcore/core.hpp>
#include <libdevice/device.hpp>
#include <libmechanism/mechanism.hpp>
#include <libutil/util.hpp>

#include "action.hpp"

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
  std::cout << "Starting machine ..." << std::endl;

  // preparing shutdown hook
  std::atexit(shutdown_hook);

  ATM_STATUS status = ATM_OK;

  // initialize `core` such as config, logger, and state
  status = initialize_core();
  if (status == ATM_ERR) {
    throw std::runtime_error(
        "Failed to initialize `core` functionality, something is wrong");
  }

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
void homing::operator()(Event const&, FSM&, SourceState&, TargetState&) const {
  act();
}

namespace spraying {
template <typename Event,
          typename FSM,
          typename SourceState,
          typename TargetState>
void job::operator()(Event const&, FSM& fsm, SourceState&, TargetState&) const {
  massert(device::DigitalOutputDeviceRegistry::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  auto&& movement = mechanism::movement_mechanism();

  LOG_INFO("Spraying...");
  fsm.spraying_running->write(device::digital::value::high);

  LOG_INFO("Moving to spraying position...");
  movement->move_to_spraying_position();

  sleep_for<time_units::millis>(3000);

  LOG_INFO("Turning on the spray...");
  fsm.spray->write(device::digital::value::high);

  sleep_for<time_units::millis>(3000);

  LOG_INFO("Follow spraying paths...");
  movement->follow_spraying_paths();

  fsm.spray->write(device::digital::value::low);

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
  fsm.spraying_ready->write(device::digital::value::high);
  fsm.spraying_running->write(device::digital::value::low);
  fsm.spraying_complete->write(device::digital::value::high);

  LOG_INFO("Spraying is completed...");

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
  massert(device::DigitalOutputDeviceRegistry::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");
  massert(Config::get() != nullptr, "sanity");

  auto*  config = Config::get();
  auto&& movement = mechanism::movement_mechanism();

  LOG_INFO("Tending begins...");
  fsm.tending_running->write(device::digital::value::high);

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

  if (fsm.finger->duty_cycle(config->finger<unsigned int>("duty-cycle")) ==
      ATM_ERR) {
    LOG_INFO("Cannot set finger duty cycle...");
  }

  sleep_for<time_units::millis>(1000);

  LOG_INFO("Follow zigzag paths...");
  movement->follow_tending_paths_zigzag();

  sleep_for<time_units::millis>(1000);

  LOG_INFO("Turning off the motor...");
  if (fsm.finger->duty_cycle(0) == ATM_ERR) {
    LOG_INFO("Cannot set finger duty cycle...");
  }

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
  fsm.tending_ready->write(device::digital::value::high);
  fsm.tending_running->write(device::digital::value::low);
  fsm.tending_complete->write(device::digital::value::high);

  LOG_INFO("Tending is completed...");

  sleep_for<time_units::millis>(1000);
  root_machine(fsm).task_completed();
}
}  // namespace tending
}  // namespace action

NAMESPACE_END

#endif  // APP_ACTION_INLINE_HPP_
