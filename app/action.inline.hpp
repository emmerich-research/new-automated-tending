#ifndef APP_ACTION_INLINE_HPP_
#define APP_ACTION_INLINE_HPP_

#include <libcore/core.hpp>

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

  auto&&        movement = mechanism::movement_mechanism();
  guard::homing homing;

  while (!homing.check()) {
    movement->homing();
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
