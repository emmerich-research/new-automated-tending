#include "precompiled.hpp"

#include "test.hpp"

#include <spdlog/spdlog.h>

#include "state.hpp"

int test(int a, int b) {
  spdlog::info("hello");
  emmerich::State::create();
  emmerich::State::get()->print();
  return a + b;
}
