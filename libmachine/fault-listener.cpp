#include "machine.hpp"

#include "fault-listener.hpp"

#include <libdevice/device.hpp>

NAMESPACE_BEGIN

namespace machine {
FaultListener::FaultListener(tending* tsm) : tsm_{tsm} {}

FaultListener::~FaultListener() {
  running_ = false;
  if (thread().joinable()) {
    thread().join();
  }
}

void FaultListener::start() {
  massert(tsm()->is_ready(), "sanity");

  std::lock_guard<std::mutex> lock(mutex());

  if (!running() && tsm()->is_ready()) {
    LOG_INFO("Starting fault listener");
    running_ = true;
    thread_ = std::thread(&FaultListener::execute, this);
  }
}

void FaultListener::stop() {
  massert(tsm()->is_ready(), "sanity");

  std::lock_guard<std::mutex> lock(mutex());

  if (running() && tsm()->is_ready()) {
    LOG_INFO("Stopping fault listener");
    running_ = false;
    if (thread().joinable()) {
      thread().join();
    }
    LOG_INFO("Stopping fault listener complete");
  }
}

void FaultListener::execute() {
  massert(State::get() != nullptr, "sanity");
  massert(device::DigitalInputDeviceRegistry::get() != nullptr, "sanity");
  massert(tsm()->is_ready(), "sanity");

  auto* state = State::get();
  auto* digital_input_registry = device::DigitalInputDeviceRegistry::get();

  auto&& limit_switch_x =
      digital_input_registry->get(device::id::limit_switch::x());
  auto&& limit_switch_y =
      digital_input_registry->get(device::id::limit_switch::y());
  auto&& limit_switch_z1 =
      digital_input_registry->get(device::id::limit_switch::z1());
  auto&& limit_switch_z2 =
      digital_input_registry->get(device::id::limit_switch::z2());
  auto&& finger_protection = digital_input_registry->get(
      device::id::limit_switch::finger_protection());
  auto&& spraying_tending_height = digital_input_registry->get(
      device::id::comm::plc::spraying_tending_height());
  auto&& cleaning_height =
      digital_input_registry->get(device::id::comm::plc::cleaning_height());
  auto&& e_stop = digital_input_registry->get(device::id::comm::plc::e_stop());

  while (running()) {
    while (running() && (tsm()->is_no_task() || state->fault())) {
    }

    if (!running()) {
      return;
    }

    // case 1: e-stop button is pressed
    if (e_stop->read_bool()) {
      state->fault(true);
      tsm()->fault();
    }

    // case 2: limit switches are turning on while moving
    //         except for homing
    if (!state->homing() &&
        (limit_switch_x->read_bool() || limit_switch_y->read_bool() ||
         limit_switch_z1->read_bool() || limit_switch_z2->read_bool())) {
      state->fault(true);
      tsm()->fault();
    }

    // case 3: height is changing while running
    // case 3.1: at tending and spraying, check the height
    //           and the special limit switch for checking the finger
    if (state->spraying_running() || state->tending_running()) {
      if (!spraying_tending_height->read_bool()) {
        state->fault(true);
        tsm()->fault();
      }

      if (finger_protection->read_bool()) {
        state->fault(true);
        tsm()->fault();
      }
    }

    // case 3.2: at tending and spraying height
    if (state->cleaning_running()) {
      if (!cleaning_height->read_bool()) {
        state->fault(true);
        tsm()->fault();
      }
    }
  }
}
}  // namespace machine

NAMESPACE_END
