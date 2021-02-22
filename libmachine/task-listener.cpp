#include "machine.hpp"

#include "task-listener.hpp"

#include <thread>

#include <libdevice/device.hpp>
#include <libutil/util.hpp>

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
    // LOG_INFO("Task listener is stopped completely");
  }
}

void TaskListener::execute() {
  massert(Config::get() != nullptr, "sanity");
  massert(State::get() != nullptr, "sanity");
  massert(tsm()->is_ready(), "sanity");

  auto* state = State::get();
  auto* config = Config::get();

  time_unit start = seconds();
  time_unit end = seconds();

  while (running() && state->running()) {
    while (state->running() && !state->homing()) {
      if (!state->running()) {
        return;
      }

      sleep_for<time_units::millis>(50);
    }
    // {
    //   std::unique_lock<std::mutex> lock(mutex());
    //   state->signal().wait(
    //       lock, [state] { return !state->running() || state->homing(); });
    // }

    if (!running() || !state->running()) {
      return;
    }

    if (state->homing()) {
      start = seconds();
      end = seconds();
    }

    if (!running()) {
      return;
    }

    while (running() && state->homing() &&
           ((end - start) < config->timeout())) {
      end = seconds();
    }

    if (!running()) {
      return;
    }

    if (state->homing()) {
      LOG_ERROR("[FAULT] Timeout while doing task!");
      if (state->spraying_running()) {
        LOG_ERROR("[FAULT] Last task: spraying");
      } else if (state->tending_running()) {
        LOG_ERROR("[FAULT] Last task: tending");
      } else if (state->cleaning_running()) {
        LOG_ERROR("[FAULT] Last task: cleaning");
      }
      state->homing(false);
      state->fault(true);
      tsm()->fault();
    } else {
      LOG_INFO("Homing task took about {} seconds", end - start);
    }
  }
}
}  // namespace machine

NAMESPACE_END
