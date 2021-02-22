#include "mechanism.hpp"

#include "liquid-refilling.hpp"

#include <libdevice/device.hpp>
#include <libutil/util.hpp>

NAMESPACE_BEGIN

namespace mechanism {
namespace impl {
LiquidRefillingImpl::LiquidRefillingImpl()
    : active_{false},
      water_draining_time_{30},
      disinfectant_draining_time_{30} {}

LiquidRefillingImpl::~LiquidRefillingImpl() {}

void LiquidRefillingImpl::setup_water_device(const std::string& level_device_id,
                                             const std::string& in_device_id,
                                             const std::string& out_device_id) {
  massert(device::ShiftRegister::get() != nullptr, "sanity");
  massert(device::FloatDeviceRegistry::get() != nullptr, "sanity");

  auto* float_device_registry = device::FloatDeviceRegistry::get();
  auto* shift_register = device::ShiftRegister::get();

  auto&& water_level_device = float_device_registry->get(level_device_id);

  if (!water_level_device) {
    active_ = false;
    return;
  }

  water_level_device_ = water_level_device;

  if (!shift_register->exist(in_device_id) ||
      !shift_register->exist(out_device_id)) {
    active_ = false;
    return;
  }

  water_in_device_id_ = in_device_id;
  water_out_device_id_ = out_device_id;
}

void LiquidRefillingImpl::setup_disinfectant_device(
    const std::string& level_device_id,
    const std::string& in_device_id,
    const std::string& out_device_id) {
  massert(device::ShiftRegister::get() != nullptr, "sanity");
  massert(device::FloatDeviceRegistry::get() != nullptr, "sanity");

  auto* float_device_registry = device::FloatDeviceRegistry::get();
  auto* shift_register = device::ShiftRegister::get();

  auto&& disinfectant_level_device =
      float_device_registry->get(level_device_id);
  disinfectant_level_device_ = disinfectant_level_device;

  if (!disinfectant_level_device) {
    active_ = false;
    return;
  }

  disinfectant_level_device_ = disinfectant_level_device;

  if (!shift_register->exist(in_device_id) ||
      !shift_register->exist(out_device_id)) {
    active_ = false;
    return;
  }

  disinfectant_in_device_id_ = in_device_id;
  disinfectant_out_device_id_ = out_device_id;
}

void LiquidRefillingImpl::setup_draining_time(
    unsigned int water_draining_time,
    unsigned int disinfectant_draining_time) {
  water_draining_time_ = water_draining_time;
  disinfectant_draining_time_ = disinfectant_draining_time;
}

liquid::status LiquidRefillingImpl::water_level() const {
  return water_level_device()->read();
}

liquid::status LiquidRefillingImpl::disinfectant_level() const {
  return disinfectant_level_device()->read();
}

void LiquidRefillingImpl::exchange_water() const {
  massert(State::get() != nullptr, "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");

  auto* state = State::get();
  auto* shift_register = device::ShiftRegister::get();

  LOG_INFO("Starting to exchange water");

  if (state->fault()) {
    return;
  }

  state->water_refilling_running(true);

  if (state->fault()) {
    return;
  }

  // ------------------
  // Draining
  // ------------------

  LOG_DEBUG("Draining water");
  shift_register->write(water_out_device_id(), device::digital::value::high);

  if (state->fault()) {
    return;
  }

  sleep_for<time_units::seconds>(1);

  if (state->fault()) {
    return;
  }

  while (!state->fault() && (water_level() != liquid::status::low)) {
    // waiting...
    sleep_for<time_units::millis>(100);
    if (state->fault()) {
      return;
    }
  }

  // this should be on liquid::status::low
  sleep_for<time_units::seconds>(water_draining_time_);

  if (state->fault()) {
    return;
  }

  shift_register->write(water_out_device_id(), device::digital::value::low);
  LOG_DEBUG("Draining water is completed");

  // ------------------
  // Refilling
  // ------------------

  if (state->fault()) {
    return;
  }

  LOG_DEBUG("Refilling water");
  shift_register->write(water_in_device_id(), device::digital::value::high);

  if (state->fault()) {
    return;
  }

  sleep_for<time_units::seconds>(1);

  if (state->fault()) {
    return;
  }

  while (!state->fault() && (water_level() != liquid::status::high)) {
    // waiting...
    sleep_for<time_units::millis>(100);
    if (state->fault()) {
      return;
    }
  }

  if (state->fault()) {
    return;
  }

  shift_register->write(water_in_device_id(), device::digital::value::low);
  LOG_DEBUG("Refilling water is completed");

  if (state->fault()) {
    return;
  }

  state->water_refilling_running(false);

  LOG_INFO("Exchanging water is finished");
}

void LiquidRefillingImpl::exchange_disinfectant() const {
  massert(State::get() != nullptr, "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");

  auto* state = State::get();
  auto* shift_register = device::ShiftRegister::get();

  LOG_INFO("Starting to exchange disinfectant");

  if (state->fault()) {
    return;
  }

  state->disinfectant_refilling_running(true);

  if (state->fault()) {
    return;
  }

  // ------------------
  // Draining
  // ------------------

  LOG_DEBUG("Draining disinfectant");
  shift_register->write(disinfectant_out_device_id(),
                        device::digital::value::high);

  if (state->fault()) {
    return;
  }

  sleep_for<time_units::seconds>(1);

  if (state->fault()) {
    return;
  }

  while (!state->fault() && (disinfectant_level() != liquid::status::low)) {
    // waiting...
    sleep_for<time_units::millis>(100);
    if (state->fault()) {
      return;
    }
  }

  // this should be on liquid::status::low
  sleep_for<time_units::seconds>(disinfectant_draining_time_);

  if (state->fault()) {
    return;
  }

  shift_register->write(disinfectant_out_device_id(),
                        device::digital::value::low);
  LOG_DEBUG("Draining disinfectant is completed");

  // ------------------
  // Refilling
  // ------------------

  if (state->fault()) {
    return;
  }

  LOG_DEBUG("Refilling disinfectant");
  shift_register->write(disinfectant_in_device_id(),
                        device::digital::value::high);

  if (state->fault()) {
    return;
  }

  sleep_for<time_units::seconds>(1);

  if (state->fault()) {
    return;
  }

  while (!state->fault() && (disinfectant_level() != liquid::status::high)) {
    // waiting...
    sleep_for<time_units::millis>(100);
    if (state->fault()) {
      return;
    }
  }

  if (state->fault()) {
    return;
  }

  shift_register->write(disinfectant_in_device_id(),
                        device::digital::value::low);
  LOG_DEBUG("Refilling disinfectant is completed");

  if (state->fault()) {
    return;
  }

  state->disinfectant_refilling_running(false);

  LOG_INFO("Exchanging disinfectant is finished");
}
}  // namespace impl
}  // namespace mechanism

NAMESPACE_END
