#include "machine.hpp"

#include "restart-fault-listener.hpp"

#include <libutil/util.hpp>

NAMESPACE_BEGIN

namespace machine {
RestartFaultListener::RestartFaultListener(tending* tsm) : tsm_{tsm} {}

RestartFaultListener::~RestartFaultListener() {
  running_ = false;
  if (thread().joinable()) {
    thread().join();
  }
}

void RestartFaultListener::start() {
  massert(tsm()->is_ready(), "sanity");

  std::lock_guard<std::mutex> lock(mutex());

  if (!running() && tsm()->is_ready()) {
    LOG_INFO("Starting restart from fault listener");
    running_ = true;
    thread_ = std::thread(&RestartFaultListener::execute, this);
  }
}

void RestartFaultListener::stop() {
  massert(tsm()->is_ready(), "sanity");

  std::lock_guard<std::mutex> lock(mutex());

  if (running() && tsm()->is_ready()) {
    LOG_INFO("Stopping restart from fault listener");
    running_ = false;
  }
}

void RestartFaultListener::execute() {
  massert(State::get() != nullptr, "sanity");
  massert(tsm()->is_ready(), "sanity");
  massert(device::DigitalInputDeviceRegistry::get() != nullptr, "sanity");

  auto* state = State::get();
  auto* digital_input_registry = device::DigitalInputDeviceRegistry::get();

  auto&& reset = digital_input_registry->get(device::id::comm::plc::reset());

  while (running() && state->running()) {
    {
      std::unique_lock<std::mutex> lock(mutex());
      state->signal().wait(
          lock, [this, state] { return !state->running() || state->fault(); });
    }

    if (!running() || !state->running()) {
      return;
    }

    while (!reset->read_bool()) {
      if (state->fault()) {
        // sleep a bit
        sleep_for<time_units::millis>(50);
      } else {
        // break if fault is changed from other threads
        break;
      }
    }

    if (state->fault()) {
      // restart is pressed
      state->homing(false);
      state->fault(false);
      tsm()->restart();
    }
    // else there are threads that win the restart condition
    // we need to break
  }
}
}  // namespace machine

NAMESPACE_END
