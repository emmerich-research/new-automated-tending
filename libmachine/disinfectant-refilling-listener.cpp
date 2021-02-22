#include "machine.hpp"

#include "disinfectant-refilling-listener.hpp"

#include <libmechanism/mechanism.hpp>
#include <libutil/util.hpp>

NAMESPACE_BEGIN

namespace machine {
DisinfectantRefillingListener::DisinfectantRefillingListener(tending* tsm)
    : tsm_{tsm} {}

DisinfectantRefillingListener::~DisinfectantRefillingListener() {
  running_ = false;
  if (thread().joinable()) {
    thread().join();
  }
}

void DisinfectantRefillingListener::start() {
  massert(tsm()->is_ready(), "sanity");

  std::lock_guard<std::mutex> lock(mutex());

  if (!running() && tsm()->is_ready()) {
    LOG_INFO("Starting disinfectant refilling listener");
    running_ = true;
    thread_ = std::thread(&DisinfectantRefillingListener::execute, this);
  }
}

void DisinfectantRefillingListener::stop() {
  massert(tsm()->is_ready(), "sanity");

  std::lock_guard<std::mutex> lock(mutex());

  if (running() && tsm()->is_ready()) {
    LOG_INFO("Stopping disinfectant refilling listener");
    running_ = false;
  }
}

void DisinfectantRefillingListener::check() const {
  massert(State::get() != nullptr, "sanity");

  auto* state = State::get();

  if (Clock::now() > state->disinfectant_refilling_next_executed()) {
    state->disinfectant_refilling_request(true);
  }
}

void DisinfectantRefillingListener::execute() {
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
               (tsm()->is_no_task() &&
                state->disinfectant_refilling_requested() &&
                !state->disinfectant_refilling_running());
      });
    }

    if (!running() || !state->running()) {
      return;
    }

    state->disinfectant_refilling_request(false);
    state->disinfectant_refilling_running(true);
    liquid_refilling->exchange_disinfectant();
    state->disinfectant_refilling_running(false);
    state->disinfectant_refilling_last_executed(Clock::now());
  }
}
}  // namespace machine

NAMESPACE_END
