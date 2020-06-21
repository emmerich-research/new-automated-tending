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
  auto&& movement = mechanism::movement_mechanism();
  if (movement != nullptr) {
    movement->disable_motors();
  }
  destroy_device();
  destroy_core();
  std::cout << "Shutting down is completed!" << std::endl;
}

static int throw_message() {
  std::cerr << "Failed to initialize machine, something is wrong" << std::endl;
  return ATM_ERR;
}

static void do_spraying() {
  massert(Config::get() != nullptr, "sanity");
  massert(device::DigitalOutputDeviceRegistry::get() != nullptr, "sanity");
  massert(device::DigitalInputDeviceRegistry::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  auto* config = Config::get();
  auto* output_registry = device::DigitalOutputDeviceRegistry::get();
  auto* input_registry = device::DigitalInputDeviceRegistry::get();

  auto&& movement = mechanism::movement_mechanism();

  auto&& spraying_ready =
      output_registry->get(device::id::comm::pi::spraying_ready());
  auto&& spraying_running =
      output_registry->get(device::id::comm::pi::spraying_running());
  auto&& spraying_complete =
      output_registry->get(device::id::comm::pi::spraying_complete());

  auto&& spray = output_registry->get(device::id::spray());
  spraying_ready->write(device::digital::value::low);
  spraying_running->write(device::digital::value::low);
  spraying_complete->write(device::digital::value::low);

  movement->homing();

  spraying_ready->write(device::digital::value::high);
  spraying_running->write(device::digital::value::low);
  spraying_complete->write(device::digital::value::low);

  sleep_for<time_units::millis>(3000);

  spraying_running->write(device::digital::value::high);

  movement->move_to_spraying_position();

  sleep_for<time_units::millis>(3000);

  spray->write(device::digital::value::high);

  sleep_for<time_units::millis>(3000);

  movement->follow_spraying_paths();

  spray->write(device::digital::value::low);

  movement->homing();

  spraying_ready->write(device::digital::value::high);
  spraying_running->write(device::digital::value::low);
  spraying_complete->write(device::digital::value::high);
}

static void do_tending() {
  massert(Config::get() != nullptr, "sanity");
  massert(device::DigitalOutputDeviceRegistry::get() != nullptr, "sanity");
  massert(device::DigitalInputDeviceRegistry::get() != nullptr, "sanity");
  massert(device::PWMDeviceRegistry::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  auto* config = Config::get();
  auto* output_registry = device::DigitalOutputDeviceRegistry::get();
  auto* input_registry = device::DigitalInputDeviceRegistry::get();
  auto* pwm_registry = device::PWMDeviceRegistry::get();

  auto&& movement = mechanism::movement_mechanism();

  auto&& tending_ready =
      output_registry->get(device::id::comm::pi::tending_ready());
  auto&& tending_running =
      output_registry->get(device::id::comm::pi::tending_running());
  auto&& tending_complete =
      output_registry->get(device::id::comm::pi::tending_complete());

  auto&& finger = pwm_registry->get(device::id::finger());

  tending_ready->write(device::digital::value::low);
  tending_running->write(device::digital::value::low);
  tending_complete->write(device::digital::value::low);

  movement->homing();

  tending_ready->write(device::digital::value::high);
  tending_running->write(device::digital::value::low);
  tending_complete->write(device::digital::value::low);

  sleep_for<time_units::millis>(3000);

  tending_running->write(device::digital::value::high);
  movement->move_to_tending_position();
  sleep_for<time_units::millis>(3000);
  movement->move_finger_down();
  sleep_for<time_units::millis>(1000);
  movement->follow_tending_paths_edge();
  sleep_for<time_units::millis>(1000);
  if (finger->duty_cycle(config->finger<unsigned int>("duty-cycle")) ==
      ATM_ERR) {
    LOG_INFO("Cannot set finger duty cycle...");
  }

  sleep_for<time_units::millis>(1000);
  movement->follow_tending_paths_zigzag();
  sleep_for<time_units::millis>(1000);

  if (finger->write(device::digital::value::low) == ATM_ERR) {
    LOG_INFO("Cannot set finger duty cycle...");
  }
  movement->homing();

  tending_ready->write(device::digital::value::high);
  tending_running->write(device::digital::value::low);
  tending_complete->write(device::digital::value::high);
}


bool menu() {
  massert(Config::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  auto* config = Config::get();
  massert(config != nullptr, "sanity");

  auto* output_registry = device::DigitalOutputDeviceRegistry::get();
  massert(output_registry != nullptr, "sanity");

  auto* stepper_registry = device::StepperRegistry::get();
  massert(stepper_registry != nullptr, "sanity");

  auto* input_registry = device::DigitalInputDeviceRegistry::get();
  massert(input_registry != nullptr, "sanity");

  auto&& movement = mechanism::movement_mechanism();

  auto&& spraying_ready =
      output_registry->get(device::id::comm::pi::spraying_ready());
  auto&& spraying_running =
      output_registry->get(device::id::comm::pi::spraying_running());
  auto&& spraying_complete =
      output_registry->get(device::id::comm::pi::spraying_complete());

  auto&& tending_ready =
      output_registry->get(device::id::comm::pi::tending_ready());
  auto&& tending_running =
      output_registry->get(device::id::comm::pi::tending_running());
  auto&& tending_complete =
      output_registry->get(device::id::comm::pi::tending_complete());

  auto&& spray = output_registry->get(device::id::spray());

  auto&& spraying_height =
    input_registry->get(device::id::comm::plc::spraying_height());

  auto&& tending_height =
    input_registry->get(device::id::comm::plc::tending_height());

  spraying_ready->write(device::digital::value::low);
  spraying_running->write(device::digital::value::low);
  spraying_complete->write(device::digital::value::low);

  tending_ready->write(device::digital::value::low);
  tending_running->write(device::digital::value::low);
  tending_complete->write(device::digital::value::low);

  spray->write(device::digital::value::low);

  LOG_INFO("----MENU-----");
  LOG_INFO("1. Spraying with trigger");
  LOG_INFO("2. Tending with trigger");
  LOG_INFO("3. Homing");
  LOG_INFO("4. Just X");
  LOG_INFO("5. Just Y");
  LOG_INFO("6. Just Z");
  LOG_INFO("7. X and Y");
  LOG_INFO("8. Spraying and Tending trigger");
  LOG_INFO("0. Exit");

  unsigned int choice;
  std::cin >> choice;

  if (choice == 1) {
    do_spraying();
    return false;
  } else if (choice == 2) {
    do_tending();
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
  } else if (choice == 8) {
    while (true) {
      auto spraying_height_status = spraying_height->read_bool();
      auto tending_height_status = tending_height->read_bool();

      LOG_INFO("Spraying height {}, tending height {}",
               spraying_height_status, tending_height_status);

      if (spraying_height_status) {
        do_spraying();
      } else if (tending_height_status) {
        do_tending();
      }
     
      sleep_for<time_units::millis>(500);
    }
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
