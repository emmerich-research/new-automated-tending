#include "precompiled.hpp"

#include <csignal>
#include <cstdlib>
#include <iostream>

#include <libcore/core.hpp>
#include <libdevice/device.hpp>
#include <libmechanism/mechanism.hpp>

USE_NAMESPACE

// forward declaration
bool        stop = false;
static void init();
static void shutdown_hook();
// static void sigint_hook(int signal);  // for SIGINT
static int throw_message();
const bool menu(const std::shared_ptr<mechanism::Movement>& movement);

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

// static void sigint_hook([[maybe_unused]] int signal) {
//   destroy_device();
//   destroy_core();
//   exit(0);
// }

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

const bool menu() {
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  auto&& movement = mechanism::movement_mechanism();

  LOG_INFO("----MENU-----");
  LOG_INFO("1. According to path");
  LOG_INFO("2. Homing");
  LOG_INFO("3. Just X (Mechanism)");
  LOG_INFO("4. Just X (Using Stepper)");
  LOG_INFO("0. Exit");

  unsigned int choice;
  std::cin >> choice;

  if (choice == 1) {
    // try to move according to path
    for (const auto& iter : Config::get()->path()) {
      LOG_INFO("Move to x={}mm y={}mm", iter.first, iter.second);
      movement->move<mechanism::movement::unit::mm>(iter.first, iter.second,
                                                    0.0);
    }
    return false;
  } else if (choice == 2) {
    movement->homing();
    return false;
  } else if (choice == 3) {
    movement->move<mechanism::movement::unit::mm>(50.0, 0.0, 0.0);
  } else if (choice == 4) {
    auto* stepper_registry = device::StepperRegistry::get();
    massert(stepper_registry != nullptr, "sanity");
    auto&& stepper_x = stepper_registry->get(device::id::stepper::x());
    stepper_x->enable();
    stepper_x->move(50);
    stepper_x->disable();
  } else if (choice == 0) {
    return true;
  }

  return false;
}

int main() {
  std::cout << "Booting up..." << std::endl;

  init();

  ATM_STATUS status = ATM_OK;

  // initialize `core` such as config, logger, and state
  status = initialize_core();
  if (status == ATM_ERR) {
    return throw_message();
  }

  // initialize `GPIO-based` devices such as analog, digital, and PWM
  status = initialize_device();
  if (status == ATM_ERR) {
    return throw_message();
  }

  // initialize `mechanism`
  status = initialize_mechanism();
  if (status == ATM_ERR) {
    return throw_message();
  }

  do {
    stop = menu();
  } while (!stop);

  return status == ATM_OK ? 0 : -1;
}
