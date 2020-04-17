#include "precompiled.hpp"

#include <spdlog/spdlog.h>

#include "test.hpp"

#include "state.hpp"

int test(int a, int b) {
  spdlog::info("hello");
  emmerich::State state;
  state->print();
  return a + b;
}
