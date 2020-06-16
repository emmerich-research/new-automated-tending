#include "precompiled.hpp"

#include "machine.hpp"

NAMESPACE_BEGIN

void shutdown_hook() {
  std::cout << "Shutting down..." << std::endl;
  destroy_device();
  destroy_core();
  std::cout << "Shutting down is completed!" << std::endl;
}

namespace machine {
const int TendingDef::VERSION = 1;

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

bool TendingDef::is_running() {
  return rebind().is_in_state<running>();
}

bool TendingDef::is_terminated() {
  return rebind().is_in_state<terminated>();
}

void TendingDef::start_spraying() {
  rebind().process_event(event::spraying::start{});
}

void TendingDef::finish_spraying() {
  rebind().process_event(event::spraying::start{});
}
}  // namespace machine

NAMESPACE_END
