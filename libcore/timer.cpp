#include "core.hpp"

#include "timer.hpp"

#include <chrono>
#include <thread>

NAMESPACE_BEGIN

time_unit millis() {
  uint64_t ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::high_resolution_clock::now().time_since_epoch())
          .count();
  return static_cast<time_unit>(ms);
}


template<>
void safe_delay<time_units::millis>(time_unit time) {
  std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

time_unit micros() {
  uint64_t us =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::high_resolution_clock::now().time_since_epoch())
          .count();
  return static_cast<time_unit>(us);
}

template<>
void safe_delay<time_units::micros>(time_unit time) {
  std::this_thread::sleep_for(std::chrono::microseconds(time));
}


time_unit nanos() {
  uint64_t ns =
      std::chrono::duration_cast<std::chrono::nanoseconds>(
          std::chrono::high_resolution_clock::now().time_since_epoch())
          .count();
  return static_cast<time_unit>(ns);
}

template<>
void safe_delay<time_units::nanos>(time_unit time) {
  std::this_thread::sleep_for(std::chrono::nanoseconds(time));
}


NAMESPACE_END
