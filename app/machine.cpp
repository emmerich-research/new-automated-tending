#include "precompiled.hpp"

#include "machine.hpp"

NAMESPACE_BEGIN

namespace machine {
const int TendingDef::VERSION = 1;

TendingDef::TendingDef() : machine_ready_{false}, thread_pool_{1} {}

TendingDef::tending_fsm& TendingDef::rebind() {
  return static_cast<tending_fsm&>(*this);
}

TendingDef::tending_fsm const& TendingDef::rebind() const {
  return static_cast<tending_fsm const&>(*this);
}

void TendingDef::start() {
  rebind().process_event(event::start{});
}

void TendingDef::stop() {
  rebind().process_event(event::stop{});
}

bool TendingDef::is_ready() const {
  return machine_ready_;
}

bool TendingDef::is_running() const {
  return rebind().is_in_state<running>();
}

bool TendingDef::is_terminated() const {
  return rebind().is_in_state<terminated>();
}

void TendingDef::start_spraying() {
  rebind().process_event(event::spraying::start{});
}

void TendingDef::run_spraying() {
  rebind().process_event(event::spraying::run{});
}

void TendingDef::start_tending() {
  rebind().process_event(event::tending::start{});
}

void TendingDef::run_tending() {
  rebind().process_event(event::tending::run{});
}

void TendingDef::task_completed() {
  rebind().process_event(event::task_complete{});
}

void TendingDef::fault() {
  rebind().process_event(event::fault::trigger{});
}

void TendingDef::fault_manual() {
  rebind().process_event(event::fault::manual{});
}

void TendingDef::restart() {
  rebind().process_event(event::fault::restart{});
}

TendingDef::running::running()
    : is_spraying_completed_{false}, is_tending_completed_{false} {}

bool TendingDef::running::is_spraying_completed() const {
  return is_spraying_completed_;
}

bool TendingDef::running::is_tending_completed() const {
  return is_tending_completed_;
}

// spraying
TendingDef::running::spraying::spraying() : initialized{false} {}

void TendingDef::running::spraying::initialize() {
  massert(device::DigitalOutputDeviceRegistry::get() != nullptr, "sanity");

  if (!initialized) {
    // auto* digital_output_registry =
    // device::DigitalOutputDeviceRegistry::get();
    // massert(digital_output_registry != nullptr, "sanity");
    // spraying_ready = std::move(
    //     digital_output_registry->get(device::id::comm::pi::spraying_ready()));
    // spraying_running = std::move(
    //     digital_output_registry->get(device::id::comm::pi::spraying_running()));
    // spraying_complete = std::move(digital_output_registry->get(
    //     device::id::comm::pi::spraying_complete()));
    // spray = std::move(digital_output_registry->get(device::id::spray()));
    initialized = true;
  }
}

// tending
TendingDef::running::tending::tending() : initialized{false} {}

void TendingDef::running::tending::initialize() {
  massert(device::DigitalOutputDeviceRegistry::get() != nullptr, "sanity");
  massert(device::PWMDeviceRegistry::get() != nullptr, "sanity");

  if (!initialized) {
    // auto* digital_output_registry =
    // device::DigitalOutputDeviceRegistry::get(); auto* pwm_registry =
    // device::PWMDeviceRegistry::get(); massert(digital_output_registry !=
    // nullptr, "sanity"); massert(pwm_registry != nullptr, "sanity");
    // tending_ready = std::move(
    //     digital_output_registry->get(device::id::comm::pi::tending_ready()));
    // tending_running = std::move(
    //     digital_output_registry->get(device::id::comm::pi::tending_running()));
    // tending_complete = std::move(
    //     digital_output_registry->get(device::id::comm::pi::tending_complete()));
    // finger = std::move(pwm_registry->get(device::id::finger()));
    initialized = true;
  }
}

}  // namespace machine

NAMESPACE_END
