#include "precompiled.hpp"

// #include <csignal>
#include <cstdlib>
#include <iostream>

#include <libcore/core.hpp>
#include <libdevice/device.hpp>
#include <libmechanism/mechanism.hpp>

// forward declaration
bool        stop = false;
static void init();
static void shutdown_hook();
// static void       sigint_hook(int signal);  // for SIGINT
static int        throw_message();
static const bool menu(
    const std::shared_ptr<emmerich::mechanism::Movement>& movement);

static void init() {
  // struct sigaction sigint_handler;

  // sigint_handler.sa_handler = sigint_hook;
  // sigemptyset(&sigint_handler.sa_mask);
  // sigint_handler.sa_flags = 0;

  // if (sigaction(SIGINT, &sigint_handler, NULL) != 0)
  //   std::perror("sigaction");

  // preparing shutdown hook
  std::atexit(shutdown_hook);
}

static void shutdown_hook() {
  stop = true;
  std::cout << "Shutting down..." << std::endl;
  emmerich::destroy_device();
  emmerich::destroy_core();
  std::cout << "Shutting down is completed!" << std::endl;
}

// static void sigint_hook(int signal) {
//   std::cout << "Use 4 to exit!" << std::endl;
// }

static int throw_message() {
  std::cerr << "Failed to initialize machine, something is wrong" << std::endl;
  return emmerich::ATM_ERR;
}

static const bool menu(
    const std::shared_ptr<emmerich::mechanism::Movement>& movement) {
  std::cout << "Run stepper" << std::endl;
  std::cout << "1. X-axis" << std::endl;
  std::cout << "2. Y-axis" << std::endl;
  std::cout << "3. Z-axis" << std::endl;
  std::cout << "4. Homing" << std::endl;
  std::cout << "0. Exit" << std::endl;

  unsigned int choice;
  std::cin >> choice;

  if (choice == 1) {
    return false;
  } else if (choice == 2) {
    return false;
  } else if (choice == 3) {
    return false;
  } else if (choice == 4) {
    movement->homing();
    return false;
  } else if (choice == 0) {
    return true;
  }

  return false;
}

int main() {
  std::cout << "Booting up..." << std::endl;

  init();

  emmerich::ATM_STATUS status = emmerich::ATM_OK;

  // initialize `core` such as config, logger, and state
  status = emmerich::initialize_core();
  if (status == emmerich::ATM_ERR) {
    return throw_message();
  }

  // initialize `GPIO-based` devices such as analog, digital, and PWM
  status = emmerich::initialize_device();
  if (status == emmerich::ATM_ERR) {
    return throw_message();
  }

  // init movement mechanism
  status = emmerich::mechanism::MovementBuilder::create();
  if (status == emmerich::ATM_ERR) {
    return throw_message();
  }

  auto movement_builder = emmerich::mechanism::MovementBuilder::get();
  status = movement_builder->setup_x("STEPPER_X", 10, "LIMIT_X");
  if (status == emmerich::ATM_ERR) {
    return throw_message();
  }

  status = movement_builder->setup_y("STEPPER_Y", 10, "LIMIT_Y");
  if (status == emmerich::ATM_ERR) {
    return throw_message();
  }

  status = movement_builder->setup_z("STEPPER_Z", 10, "LIMIT_Z");
  if (status == emmerich::ATM_ERR) {
    return throw_message();
  }

  auto movement = movement_builder->build();

  massert(movement->active(), "sanity");

  movement->move<emmerich::mechanism::movement::unit::mm>(100, 50, 10);

  do {
    stop = menu(movement);
  } while (!stop);

  return status == emmerich::ATM_OK ? 0 : -1;
}
