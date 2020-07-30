#include "machine.hpp"

#include "water-refilling-listener.hpp"

#include <libmechanism/mechanism.hpp>
#include <libutil/util.hpp>

NAMESPACE_BEGIN

namespace machine {
WaterRefillingListener::WaterRefillingListener(tending* tsm) : tsm_{tsm} {}

WaterRefillingListener::~WaterRefillingListener() {
  running_ = false;
  if (thread().joinable()) {
    thread().join();
  }
}

void WaterRefillingListener::start() {
  massert(tsm()->is_ready(), "sanity");

  std::lock_guard<std::mutex> lock(mutex());

  if (!running() && tsm()->is_ready()) {
    LOG_INFO("Starting water refilling listener");
    running_ = true;
    thread_ = std::thread(&WaterRefillingListener::execute, this);
  }
}

void WaterRefillingListener::stop() {
  massert(tsm()->is_ready(), "sanity");

  std::lock_guard<std::mutex> lock(mutex());

  if (running() && tsm()->is_ready()) {
    LOG_INFO("Stopping water refilling listener");
    running_ = false;
  }
}

void WaterRefillingListener::check() const {
  massert(State::get() != nullptr, "sanity");

  auto* state = State::get();

  if (Clock::now() > state->water_refilling_next_executed()) {
    state->water_refilling_request(true);
  }
}

void WaterRefillingListener::execute() {
  massert(State::get() != nullptr, "sanity");
  massert(tsm()->is_ready(), "sanity");
  massert(mechanism::LiquidRefilling::get() != nullptr, "sanity");

  auto* state = State::get();
  auto* liquid_refilling = mechanism::LiquidRefilling::get();

  while (running() && state->running()) {
    {
      std::unique_lock<std::mutex> lock(mutex());
      state->signal().wait(lock, [this, state] {
        return !state->running() ||
               (tsm()->is_no_task() && state->water_refilling_requested() &&
                !state->water_refilling_running());
      });
    }

    if (!running() || !state->running()) {
      return;
    }

    state->water_refilling_request(false);
    state->water_refilling_running(true);
    liquid_refilling->exchange_water();
    state->water_refilling_running(false);
    state->water_refilling_last_executed(Clock::now());
  }
}
}  // namespace machine

NAMESPACE_END
