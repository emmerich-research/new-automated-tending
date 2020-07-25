#include <algorithm>
#include <array>
#include <iostream>
#include <string>

#include <libcore/core.hpp>
#include <libdevice/device.hpp>
#include <libutil/util.hpp>

USE_NAMESPACE;

// forward declarations
static ATM_STATUS init();
static void       shutdown_hook();
static int        throw_message();

static ATM_STATUS init() {
  // initialize logger
  if (Logger::create() == ATM_ERR) {
    return ATM_ERR;
  }

  // initialize config
  if (Config::create(PROJECT_CONFIG_FILE) == ATM_ERR) {
    LOG_ERROR("Failed to load configuration");
    return ATM_ERR;
  }

  // re-init logger based on config
  Logger::get()->init(Config::get());

  // init state
  if (State::create() == ATM_ERR) {
    LOG_ERROR("Failed to initialize state");
    return ATM_ERR;
  }

  // initialize `GPIO-based` devices such as analog, digital, and PWM
  if (initialize_device() == ATM_ERR) {
    return ATM_ERR;
  }

  return ATM_OK;
}

static void shutdown_hook() {
  std::cout << "Shutting down..." << std::endl;
  destroy_device();
  destroy_core();
  std::cout << "Shutting down is completed!" << std::endl;
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

  std::array<byte, 16> ids = {0, 1, 2,  3,  4,  5,  6,  7,
                              8, 9, 10, 11, 12, 13, 14, 15};

  std::for_each(ids.begin(), ids.end(), [shift_register](const byte& id) {
    shift_register->assign(std::to_string(id), id, false);
    shift_register->write(std::to_string(id), device::digital::value::low);
  });

  std::for_each(ids.begin(), ids.end(), [shift_register](const byte& id) {
    // turn on
    shift_register->write(std::to_string(id), device::digital::value::high);
    sleep_for<time_units::millis>(100);
  });

  LOG_INFO("Wait for 2 seconds");
  sleep_for<time_units::millis>(2000);

  std::for_each(ids.begin(), ids.end(), [shift_register](const byte& id) {
    // turn off
    shift_register->write(std::to_string(id), device::digital::value::low);
    sleep_for<time_units::millis>(100);
  });

  shutdown_hook();

  return status;
}
