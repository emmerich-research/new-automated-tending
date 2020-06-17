#include "precompiled.hpp"

#include <csignal>
#include <cstdlib>
#include <iostream>

#include <libcore/core.hpp>
#include <libdevice/device.hpp>
#include <libmechanism/mechanism.hpp>
#include <libutil/util.hpp>

USE_NAMESPACE

// forward declaration
bool        stop = false;
static void init();
static void shutdown_hook();
// static void sigint_hook(int signal);  // for SIGINT
static int throw_message();
bool       menu(const std::shared_ptr<mechanism::Movement>& movement);

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

bool menu() {
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  auto* stepper_registry = device::StepperRegistry::get();
  massert(stepper_registry != nullptr, "sanity");

  auto&& movement = mechanism::movement_mechanism();

  LOG_INFO("----MENU-----");
  LOG_INFO("1. Spraying path");
  LOG_INFO("2. Tending path");
  LOG_INFO("3. Homing");
  LOG_INFO("4. Just X");
  LOG_INFO("5. Just Y");
  LOG_INFO("6. Just Z");
  LOG_INFO("7. X and Y");
  LOG_INFO("0. Exit");

  unsigned int choice;
  std::cin >> choice;

  if (choice == 1) {
    movement->move_to_spraying_position();
    movement->follow_spraying_paths();
    return false;
  } else if (choice == 2) {
    movement->move_to_tending_position();
    movement->follow_tending_paths();
    return false;
  } else if (choice == 3) {
    movement->homing();
    return false;
  } else if (choice == 4) {
    auto&& stepper_x = stepper_registry->get(device::id::stepper::x());
    stepper_x->enable();
    for (size_t i = 0; i < 2; ++i) {
      stepper_x->move(10000);
      sleep_for<time_units::millis>(200);
      stepper_x->move(-10000);
    }
    stepper_x->disable();
  } else if (choice == 5) {
    auto&& stepper_y = stepper_registry->get(device::id::stepper::y());
    stepper_y->enable();
    for (size_t i = 0; i < 2; ++i) {
      stepper_y->move(10000);
      sleep_for<time_units::millis>(200);
      stepper_y->move(-10000);
    }
    stepper_y->disable();
  } else if (choice == 6) {
    auto&& stepper_z = stepper_registry->get(device::id::stepper::z());
    stepper_z->enable();
    for (size_t i = 0; i < 2; ++i) {
      stepper_z->move(500);
      sleep_for<time_units::millis>(200);
      stepper_z->move(-500);
    }
    stepper_z->disable();
  } else if (choice == 7) {
    movement->move<mechanism::movement::unit::mm>(50.0, 50.0, 0.0);
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
