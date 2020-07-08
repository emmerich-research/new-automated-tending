#include <csignal>
#include <cstdlib>
#include <iostream>

#include <libcore/core.hpp>
#include <libdevice/device.hpp>
#include <libmechanism/mechanism.hpp>
#include <libutil/util.hpp>

USE_NAMESPACE;

// forward declaration
static bool       stop = false;
static ATM_STATUS init();
static void       shutdown_hook();
static int        throw_message();
static bool       menu();

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

  // initialize `mechanism`
  status = initialize_mechanism();
  if (status == ATM_ERR) {
    return status;
  }

  return status;
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
  massert(device::ShiftRegister::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  auto*  shift_register = device::ShiftRegister::get();
  auto&& movement = mechanism::movement_mechanism();

  shift_register->write(device::id::comm::pi::spraying_ready(),
                        device::digital::value::low);
  shift_register->write(device::id::comm::pi::spraying_running(),
                        device::digital::value::low);
  shift_register->write(device::id::comm::pi::spraying_complete(),
                        device::digital::value::low);

  movement->homing();

  shift_register->write(device::id::comm::pi::spraying_ready(),
                        device::digital::value::high);
  shift_register->write(device::id::comm::pi::spraying_running(),
                        device::digital::value::low);
  shift_register->write(device::id::comm::pi::spraying_complete(),
                        device::digital::value::low);

  sleep_for<time_units::millis>(3000);
  shift_register->write(device::id::comm::pi::spraying_running(),
                        device::digital::value::high);

  movement->move_to_spraying_position();

  sleep_for<time_units::millis>(3000);

  shift_register->write(device::id::spray(), device::digital::value::high);

  sleep_for<time_units::millis>(3000);

  movement->follow_spraying_paths();

  shift_register->write(device::id::spray(), device::digital::value::low);

  movement->homing();

  shift_register->write(device::id::comm::pi::spraying_ready(),
                        device::digital::value::high);
  shift_register->write(device::id::comm::pi::spraying_running(),
                        device::digital::value::low);
  shift_register->write(device::id::comm::pi::spraying_complete(),
                        device::digital::value::high);
}

static void do_tending() {
  massert(Config::get() != nullptr, "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");
  massert(device::PWMDeviceRegistry::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  auto*  config = Config::get();
  auto*  shift_register = device::ShiftRegister::get();
  auto*  pwm_registry = device::PWMDeviceRegistry::get();
  auto&& movement = mechanism::movement_mechanism();
  auto&& finger = pwm_registry->get(device::id::finger());

  shift_register->write(device::id::comm::pi::tending_ready(),
                        device::digital::value::low);
  shift_register->write(device::id::comm::pi::tending_running(),
                        device::digital::value::low);
  shift_register->write(device::id::comm::pi::tending_complete(),
                        device::digital::value::low);

  movement->homing();

  shift_register->write(device::id::comm::pi::tending_ready(),
                        device::digital::value::high);
  shift_register->write(device::id::comm::pi::tending_running(),
                        device::digital::value::low);
  shift_register->write(device::id::comm::pi::tending_complete(),
                        device::digital::value::low);

  sleep_for<time_units::millis>(3000);

  shift_register->write(device::id::comm::pi::tending_running(),
                        device::digital::value::high);

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

  shift_register->write(device::id::comm::pi::tending_ready(),
                        device::digital::value::high);
  shift_register->write(device::id::comm::pi::tending_running(),
                        device::digital::value::low);
  shift_register->write(device::id::comm::pi::tending_complete(),
                        device::digital::value::high);
}

static bool menu() {
  massert(Config::get() != nullptr, "sanity");
  massert(State::get() != nullptr, "sanity");
  massert(device::DigitalInputDeviceRegistry::get() != nullptr, "sanity");
  massert(device::DigitalOutputDeviceRegistry::get() != nullptr, "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");
  massert(device::StepperRegistry::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  auto*  config = Config::get();
  auto*  state = State::get();
  auto*  shift_register = device::ShiftRegister::get();
  auto*  stepper_registry = device::StepperRegistry::get();
  auto*  input_registry = device::DigitalInputDeviceRegistry::get();
  auto&& movement = mechanism::movement_mechanism();

  auto&& spraying_tending_height =
      input_registry->get(device::id::comm::plc::spraying_tending_height());
  auto&& cleaning_height =
      input_registry->get(device::id::comm::plc::cleaning_height());

  shift_register->write(device::id::comm::pi::spraying_ready(),
                        device::digital::value::low);
  shift_register->write(device::id::comm::pi::spraying_running(),
                        device::digital::value::low);
  shift_register->write(device::id::comm::pi::spraying_complete(),
                        device::digital::value::low);

  shift_register->write(device::id::comm::pi::tending_ready(),
                        device::digital::value::low);
  shift_register->write(device::id::comm::pi::tending_running(),
                        device::digital::value::low);
  shift_register->write(device::id::comm::pi::tending_complete(),
                        device::digital::value::low);

  shift_register->write(device::id::spray(), device::digital::value::low);

  LOG_INFO("----MENU----");
  LOG_INFO("1.   Spraying and Tending");
  LOG_INFO("2.   Spraying");
  LOG_INFO("3.   Tending");
  LOG_INFO("4.   Cleaning");
  LOG_INFO("5.   Homing");
  LOG_INFO("6.   Just X");
  LOG_INFO("7.   Just Y");
  LOG_INFO("8.   Just Z");
  LOG_INFO("9.   Spraying and Tending trigger");
  LOG_INFO("10.  Move X with specified distance (in cm, can be negative)");
  LOG_INFO("11.  Move Y with specified distance (in cm, can be negative)");
  LOG_INFO("12.  Set speed <fast/normal/slow>");
  LOG_INFO("0.   Exit");

  unsigned int choice;
  std::cin >> choice;

  if (choice == 1) {
    do_spraying();
    do_tending();
  } else if (choice == 2) {
    do_spraying();
  } else if (choice == 3) {
    do_tending();
  } else if (choice == 4) {
    // TODO: implementing cleaning
  } else if (choice == 5) {
    movement->homing();
  } else if (choice == 6) {
    auto&& stepper_x = stepper_registry->get(device::id::stepper::x());
    stepper_x->enable();
    for (size_t i = 0; i < 2; ++i) {
      stepper_x->move(10000);
      sleep_for<time_units::millis>(200);
      stepper_x->move(-10000);
    }
    stepper_x->disable();
  } else if (choice == 7) {
    auto&& stepper_y = stepper_registry->get(device::id::stepper::y());
    stepper_y->enable();
    for (size_t i = 0; i < 2; ++i) {
      stepper_y->move(10000);
      sleep_for<time_units::millis>(200);
      stepper_y->move(-10000);
    }
    stepper_y->disable();
  } else if (choice == 8) {
    auto&& stepper_z = stepper_registry->get(device::id::stepper::z());
    stepper_z->enable();
    for (size_t i = 0; i < 2; ++i) {
      stepper_z->move(500);
      sleep_for<time_units::millis>(200);
      stepper_z->move(-500);
    }
    stepper_z->disable();
  } else if (choice == 9) {
    while (true) {
      auto spraying_tending_height_status =
          spraying_tending_height->read_bool();
      auto cleaning_height_status = cleaning_height->read_bool();

      LOG_INFO("Spraying and Tending height {}, Cleaning height {}",
               spraying_tending_height_status, cleaning_height_status);

      if (spraying_tending_height_status) {
        do_spraying();

        LOG_INFO("Wait for 3 seconds to tend");
        sleep_for<time_units::seconds>(3);

        do_tending();
      } else if (cleaning_height_status) {
        // noop
      }

      sleep_for<time_units::millis>(500);
    }
  } else if (choice == 10 || choice == 11) {
    double distance;

    std::cin >> distance;

    movement->motor_profile(
        config->fault_speed_profile(state->speed_profile()));

    if (choice == 10) {
      movement->move<mechanism::movement::unit::cm>(distance, 0.0, 0.0);
    } else if (choice == 11) {
      movement->move<mechanism::movement::unit::cm>(0.0, distance, 0.0);
    }
    state->reset_coordinate();
  } else if (choice == 12) {
    std::string   speed_str;
    config::speed speed_profile = config::speed::normal;

    std::cin >> speed_str;

    if (speed_str == "fast") {
      speed_profile = config::speed::fast;
    } else if (speed_str == "normal") {
      speed_profile = config::speed::normal;
    } else if (speed_str == "slow") {
      speed_profile = config::speed::slow;
    }

    LOG_INFO("Set speed to {}", speed_str);

    state->speed_profile(speed_profile);
  } else if (choice == 0) {
    return true;
  }

  return false;
}

int main() {
  std::cout << "Booting up..." << std::endl;

  ATM_STATUS status = ATM_OK;

  status = init();
  if (status == ATM_ERR) {
    return throw_message();
  }

  do {
    stop = menu();
  } while (!stop);

  shutdown_hook();

  return status == ATM_OK ? 0 : -1;
}
