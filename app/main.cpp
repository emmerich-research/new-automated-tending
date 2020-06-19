#include "precompiled.hpp"

#include <iostream>
#include <stdexcept>

#include <libcore/core.hpp>
#include <libdevice/device.hpp>
#include <libgui/gui.hpp>

#include "machine.hpp"

USE_NAMESPACE

int throw_message() {
  std::cerr << "Failed to initialize machine, something is wrong" << std::endl;
  return ATM_ERR;
}

int main() {
  ATM_STATUS status = ATM_OK;
  // run state machine
  machine::tending tsm;

  // initialization
  try {
    tsm.start();
    massert(tsm.is_running(), "sanity");
  } catch (std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    status = ATM_ERR;
  }

  // early stopping
  if (status == ATM_ERR) {
    tsm.stop();
    massert(tsm.is_terminated(), "sanity");
    return status;
  }

  // main loop
  while (!tsm.is_terminated()) {
    // sequences:
    // 1. spraying
    // 2. tending
  }

  // stopping
  tsm.stop();
  massert(tsm.is_terminated(), "sanity");

  return ATM_OK;
}
