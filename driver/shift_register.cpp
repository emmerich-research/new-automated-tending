#include <algorithm>
#include <array>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <string>

#include <libcore/core.hpp>
#include <libdevice/device.hpp>
#include <libutil/util.hpp>

USE_NAMESPACE

// forward declarations
static ATM_STATUS init();
static int        throw_message();

// global variables
bool stop = false;

static ATM_STATUS init() {
  ATM_STATUS status = ATM_OK;

  // initialize `core` such as config, logger, and state
  status = initialize_core();
  if (status == ATM_ERR) {
    return status;
  }

  // initialize `GPIO-based` devices such as analog, digital, and PWM
  status = initialize_device();
  if (status == ATM_ERR) {
    return status;
  }

  return status;
}

static int throw_message() {
  std::cerr << "Failed to initialize machine, something is wrong" << std::endl;
  return ATM_ERR;
}

int main() {
  ATM_STATUS status = ATM_OK;

  status = init();
  if (status == ATM_ERR) {
    return throw_message();
  }

  auto* shift_register = device::ShiftRegister::get();
  // std::array<std::string, 6> ids{device::id::comm::pi::tending_ready(),
  //                                device::id::comm::pi::spraying_ready(),
  //                                device::id::comm::pi::tending_running(),
  //                                device::id::comm::pi::spraying_running(),
  //                                device::id::comm::pi::tending_complete(),
  //                                device::id::comm::pi::spraying_complete()};

  std::array<byte, 16> ids = {0, 1, 2,  3,  4,  5,  6,  7,
                              8, 9, 10, 11, 12, 13, 14, 15};

  std::for_each(ids.begin(), ids.end(), [shift_register](const byte& id) {
    shift_register->assign(std::to_string(id), id, false);
  });

  std::for_each(ids.begin(), ids.end(), [shift_register](const byte& id) {
    // turn on
    shift_register->write(std::to_string(id), device::digital::value::high);
    sleep_for<time_units::millis>(1000);
  });

  LOG_INFO("Wait for 3 seconds");
  sleep_for<time_units::millis>(3000);

  std::for_each(ids.begin(), ids.end(), [shift_register](const byte& id) {
    // turn off
    shift_register->write(std::to_string(id), device::digital::value::low);
    sleep_for<time_units::millis>(1000);
  });

  return status;
}
