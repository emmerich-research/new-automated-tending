#include <algorithm>
#include <array>
#include <exception>
#include <iostream>
#include <string>

#include <libalgo/algo.hpp>
#include <libcore/core.hpp>
#include <libdevice/device.hpp>
#include <libutil/util.hpp>

USE_NAMESPACE;

// forward declarations
static bool             stop = false;
static algo::ThreadPool thread_pool{1};
static ATM_STATUS       init();
static void             shutdown_hook();
static int              throw_message();
static bool             menu();

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
  stop = true;
  std::cout << "Shutting down..." << std::endl;
  destroy_device();
  destroy_core();
  std::cout << "Shutting down is completed!" << std::endl;
}

static int throw_message() {
  std::cerr << "Failed to initialize machine, something is wrong" << std::endl;
  return ATM_ERR;
}

static bool menu() {
  massert(Config::get() != nullptr, "sanity");
  massert(device::UltrasonicDeviceRegistry::get() != nullptr, "sanity");

  auto* config = Config::get();
  auto* ultrasonic_device_registry = device::UltrasonicDeviceRegistry::get();

  LOG_INFO("----MENU-----");
  LOG_INFO("1. Read Water Level for X seconds");
  LOG_INFO("2. Read Disinfectant Level for X seconds");
  LOG_INFO("3. Read Water/Disinfectant Level for X seconds");
  LOG_INFO("0. Exit");

  unsigned int choice;
  unsigned int duration;
  std::cin >> choice >> duration;

  auto&& water_level =
      ultrasonic_device_registry->get(device::id::ultrasonic::water_level());
  auto&& disinfectant_level = ultrasonic_device_registry->get(
      device::id::ultrasonic::disinfectant_level());

  time_unit start = seconds();
  if (choice == 1) {
    while (true) {
      LOG_DEBUG("Distance {} cm", water_level
                                      ->distance(config->ultrasonic<double>(
                                          "water-level", "max-range"))
                                      .value_or(-99.0));
      sleep_for<time_units::millis>(10);

      if ((seconds() - start) == duration) {
        break;
      }
    }
  } else if (choice == 2) {
    while (true) {
      LOG_DEBUG("Distance {} cm", disinfectant_level
                                      ->distance(config->ultrasonic<double>(
                                          "disinfectant-level", "max-range"))
                                      .value_or(-99.0));
      sleep_for<time_units::millis>(10);

      if ((seconds() - start) == duration) {
        break;
      }
    }
  } else if (choice == 3) {
    while (true) {
      LOG_DEBUG(
          "Water Level {} cm, Disinfectant Level {} cm",
          water_level
              ->distance(config->ultrasonic<double>("water-level", "max-range"))
              .value_or(-99.0),
          disinfectant_level
              ->distance(
                  config->ultrasonic<double>("disinfectant-level", "max-range"))
              .value_or(-99.0));
      sleep_for<time_units::millis>(10);

      if ((seconds() - start) == duration) {
        break;
      }
    }
  } else if (choice == 0) {
    return true;
  }

  return false;
}

int main() {
  ATM_STATUS status = ATM_OK;

  status = init();
  if (status == ATM_ERR) {
    return throw_message();
  }

  do {
    stop = menu();
  } while (!stop);

  shutdown_hook();

  return status;
}
