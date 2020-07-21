#include "machine.hpp"

#include "task-listener.hpp"

#include <libdevice/device.hpp>

NAMESPACE_BEGIN

namespace machine {
TaskListener::TaskListener(tending* tsm) : tsm_{tsm} {}

TaskListener::~TaskListener() {
  running_ = false;
  if (thread().joinable()) {
    thread().join();
  }
}

void TaskListener::start() {
  massert(tsm()->is_ready(), "sanity");

  std::lock_guard<std::mutex> lock(mutex());

  if (!running() && tsm()->is_ready()) {
    LOG_INFO("Starting task listener");
    running_ = true;
    thread_ = std::thread(&TaskListener::execute, this);
  }
}

void TaskListener::stop() {
  massert(tsm()->is_ready(), "sanity");

  std::lock_guard<std::mutex> lock(mutex());

  if (running() && tsm()->is_ready()) {
    LOG_INFO("Stopping task listener");
    running_ = false;
    if (thread().joinable()) {
      thread().join();
    }
    LOG_INFO("Task listener is stopped completely");
  }
}

void TaskListener::execute() {
  massert(State::get() != nullptr, "sanity");
  // massert(device::DigitalInputDeviceRegistry::get() != nullptr, "sanity");
  massert(tsm()->is_ready(), "sanity");

  auto* state = State::get();
  // auto* digital_input_registry = device::DigitalInputDeviceRegistry::get();

  while (running()) {
    while (running() && (!state->homing())) {
    }
  }
}
}  // namespace machine

NAMESPACE_END
