#ifndef APP_ACTION_INLINE_HPP_
#define APP_ACTION_INLINE_HPP_

#include <libcore/core.hpp>
#include <libdevice/device.hpp>
#include <libutil/util.hpp>

#include "action.hpp"

NAMESPACE_BEGIN

namespace action {
template <typename Event,
          typename FSM,
          typename SourceState,
          typename TargetState>
void do_start::operator()(Event const&,
                          FSM&,
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
}

template <typename Event,
          typename FSM,
          typename SourceState,
          typename TargetState>
void do_stop::operator()(Event const&, FSM&, SourceState&, TargetState&) const {
  shutdown_hook();
}

template <typename Event,
          typename FSM,
          typename SourceState,
          typename TargetState>
void do_homing::operator()(Event const&,
                           FSM&,
                           SourceState&,
                           TargetState&) const {
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  LOG_INFO("Homing to make sure task ready...");

  auto&&        movement = mechanism::movement_mechanism();
  guard::homing homing;

  while (!homing.check()) {
    movement->homing();
    LOG_DEBUG("Still doing homing, retrying homing for 400ms...");
    sleep_for<time_units::millis>(400);
  }
}

template <typename Event,
          typename FSM,
          typename SourceState,
          typename TargetState>
void do_spraying::operator()(Event const&,
                             FSM&,
                             SourceState&,
                             TargetState&) const {
  massert(device::DigitalOutputDeviceRegistry::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  auto*  output_registry = device::DigitalOutputDeviceRegistry::get();
  auto&& spraying_running =
      output_registry->get(device::id::comm::pi::spraying_running());
  spraying_running->write(device::digital::value::high);
  LOG_INFO("Spraying is running...");
}

template <typename Event,
          typename FSM,
          typename SourceState,
          typename TargetState>
void do_tending::operator()(Event const&,
                            FSM&,
                            SourceState&,
                            TargetState&) const {}
}  // namespace action

NAMESPACE_END

#endif  // APP_ACTION_INLINE_HPP_
