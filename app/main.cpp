#include <cstdlib>
#include <iostream>

#include <libcore/core.hpp>
#include <libdevice/device.hpp>
#include <libmechanism/mechanism.hpp>

void shutdown_hook() {
  std::cout << "Shutting down..." << std::endl;
  emmerich::destroy_device();
  emmerich::destroy_core();
  std::cout << "Shutting down is completed!" << std::endl;
}

int throw_message() {
  std::cerr << "Failed to initialize machine, something is wrong" << std::endl;
  return emmerich::ATM_ERR;
}

int main() {
  std::cout << "Booting up..." << std::endl;

  // preparing shutdown hook
  std::atexit(shutdown_hook);

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
  status = emmerich::mechanism::MovementBuilder::create(10);
  if (status == emmerich::ATM_ERR) {
    return throw_message();
  }

  auto movement_builder = emmerich::mechanism::MovementBuilder::get();
  status = movement_builder->setup_x("STEPPER_X", "LIMIT_X");
  if (status == emmerich::ATM_ERR) {
    return throw_message();
  }

  status = movement_builder->setup_y("STEPPER_Y", "LIMIT_Y");
  if (status == emmerich::ATM_ERR) {
    return throw_message();
  }

  status = movement_builder->setup_z("STEPPER_Z", "LIMIT_Z");
  if (status == emmerich::ATM_ERR) {
    return throw_message();
  }

  auto movement = movement_builder->build();

  massert(movement->active(), "sanity");

  movement->move_x<emmerich::mechanism::movement::unit::mm>(15);
  movement->move_y<emmerich::mechanism::movement::unit::mm>(5);
  movement->move_z<emmerich::mechanism::movement::unit::mm>(10);

  // TODO: run the program

  return status == emmerich::ATM_OK ? 0 : -1;
}
