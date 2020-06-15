#include "precompiled.hpp"

#include <iostream>
#include <stdexcept>

#include <libcore/core.hpp>
#include <libdevice/device.hpp>
#include <libmechanism/mechanism.hpp>

#include "machine.hpp"

USE_NAMESPACE

int throw_message() {
  std::cerr << "Failed to initialize machine, something is wrong" << std::endl;
  return ATM_ERR;
}

int main() {
  // run state machine
  // TODO: should be looping
  machine::tending tsm;
  try {
    tsm.process_event(event::start{});
    massert(tsm.is_in_state<machine::tending::running>(), "sanity");
  } catch (std::runtime_error& e) {
    tsm.process_event(event::stop{});
    std::cerr << e.what() << std::endl;
    return ATM_ERR;
  }

  tsm.process_event(event::stop{});
  massert(tsm.is_in_state<machine::tending::terminated>(), "sanity");

  return ATM_OK;
}
