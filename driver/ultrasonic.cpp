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
  massert(device::UltrasonicDeviceRegistry::get() != nullptr, "sanity");

  auto* ultrasonic_device_registry = device::UltrasonicDeviceRegistry::get();

  LOG_INFO("----MENU-----");
  LOG_INFO("1. Read Water Level for 10s");
  LOG_INFO("2. Read Disinfectant Level for 10s");
  LOG_INFO("0. Exit");

  unsigned int choice;
  std::cin >> choice;

  if (choice == 1) {
    auto&& water_level =
        ultrasonic_device_registry->get(device::id::ultrasonic::water_level());

    auto result =
        thread_pool.enqueue([water_level] { return water_level->distance(); });

    unsigned int second_count = 0;
    while (second_count != 10) {
      LOG_DEBUG("Second {}", second_count);
      second_count++;
      sleep_for<time_units::seconds>(1);
    }

    // can be busy
    thread_pool.join();

    try {
      auto res = result.get();
      LOG_DEBUG("Water Level {}", res);
    } catch (std::exception& e) {
      LOG_DEBUG("{}", e.what());
    }

    return false;
  } else if (choice == 2) {
    auto&& disinfectant_level = ultrasonic_device_registry->get(
        device::id::ultrasonic::disinfectant_level());

    auto result = thread_pool.enqueue(
        [disinfectant_level] { return disinfectant_level->distance(); });

    time_unit start = seconds();
    while (seconds() - start != 10) {
      // noop
    }

    // can be busy
    thread_pool.join();

    try {
      auto res = result.get();
      LOG_DEBUG("Disinfectant Level {}", res);
    } catch (std::exception& e) {
      LOG_DEBUG("{}", e.what());
    }

    return false;
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
