#include "mechanism.hpp"

#include "liquid-refilling.hpp"

#include <libdevice/device.hpp>
#include <libutil/util.hpp>

NAMESPACE_BEGIN

namespace mechanism {
namespace impl {
const double LiquidRefillingImpl::MinHeight = 8.0;
const double LiquidRefillingImpl::MaxHeight = 12.0;

LiquidRefillingImpl::LiquidRefillingImpl() : active_{false} {}

LiquidRefillingImpl::~LiquidRefillingImpl() {}

void LiquidRefillingImpl::setup_water_device(const std::string& level_device_id,
                                             const std::string& in_device_id,
                                             const std::string& out_device_id) {
  massert(device::ShiftRegister::get() != nullptr, "sanity");
  massert(device::UltrasonicDeviceRegistry::get() != nullptr, "sanity");

  auto* ultrasonic_device_registry = device::UltrasonicDeviceRegistry::get();
  auto* shift_register = device::ShiftRegister::get();

  auto&& water_level_device = ultrasonic_device_registry->get(level_device_id);

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
  massert(device::UltrasonicDeviceRegistry::get() != nullptr, "sanity");

  auto* ultrasonic_device_registry = device::UltrasonicDeviceRegistry::get();
  auto* shift_register = device::ShiftRegister::get();

  auto&& disinfectant_level_device =
      ultrasonic_device_registry->get(level_device_id);
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

liquid::status LiquidRefillingImpl::water_level() const {
  massert(Config::get() != nullptr, "sanity");

  auto* config = Config::get();
  auto  water_level = water_level_device()->distance(
      config->ultrasonic<double>("water-level", "max-range"));

  if (water_level) {
    double height = *water_level;

    if (height <= MinHeight) {
      return liquid::status::full;
    } else if (height >= MaxHeight) {
      return liquid::status::empty;
    } else {
      return liquid::status::normal;
    }
  }

  return liquid::status::unknown;
}

liquid::status LiquidRefillingImpl::disinfectant_level() const {
  massert(Config::get() != nullptr, "sanity");

  auto* config = Config::get();
  auto  disinfectant_level = disinfectant_level_device()->distance(
      config->ultrasonic<double>("disinfectant-level", "max-range"));

  if (disinfectant_level) {
    double height = *disinfectant_level;

    if (height <= MinHeight) {
      return liquid::status::full;
    } else if (height >= MaxHeight) {
      return liquid::status::empty;
    } else {
      return liquid::status::normal;
    }
  }

  return liquid::status::unknown;
}

void LiquidRefillingImpl::exchange_water() const {
  massert(State::get() != nullptr, "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");
  massert(device::UltrasonicDeviceRegistry::get() != nullptr, "sanity");

  auto* state = State::get();
  auto* shift_register = device::ShiftRegister::get();

  LOG_INFO("Starting to exchange water");

  state->water_refilling_running(true);

  const auto wait_until_empty = [this, state, shift_register]() {
    LOG_DEBUG("Draining water");
    shift_register->write(water_out_device_id(), device::digital::value::high);
    sleep_for<time_units::seconds>(1);

    while (!state->fault() && (water_level() != liquid::status::empty)) {
      // waiting...
      sleep_for<time_units::millis>(100);
      if (state->fault()) {
        return;
      }
    }

    shift_register->write(water_out_device_id(), device::digital::value::low);
    LOG_DEBUG("Draining water is completed");
  };

  const auto wait_until_full = [this, state, shift_register]() {
    LOG_DEBUG("Refilling water");
    shift_register->write(water_in_device_id(), device::digital::value::high);
    sleep_for<time_units::seconds>(1);

    while (!state->fault() && (water_level() != liquid::status::full)) {
      // waiting...
      sleep_for<time_units::millis>(100);
      if (state->fault()) {
        return;
      }
    }

    shift_register->write(water_in_device_id(), device::digital::value::low);
    LOG_DEBUG("Refilling water is completed");
  };

  do {
    wait_until_empty();
    if (state->fault()) {
      return;
    }

    sleep_for<time_units::seconds>(10);
    if (state->fault()) {
      return;
    }
  } while (!state->fault() && (water_level() != liquid::status::empty));

  do {
    wait_until_full();
    if (state->fault()) {
      return;
    }

    sleep_for<time_units::seconds>(10);
    if (state->fault()) {
      return;
    }
  } while (!state->fault() && (water_level() != liquid::status::full));

  state->water_refilling_running(false);

  LOG_INFO("Exchanging water is finished");
}

void LiquidRefillingImpl::exchange_disinfectant() const {
  massert(State::get() != nullptr, "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");
  massert(device::UltrasonicDeviceRegistry::get() != nullptr, "sanity");

  auto* state = State::get();
  auto* shift_register = device::ShiftRegister::get();

  LOG_INFO("Starting to exchange disinfectant");

  state->disinfectant_refilling_running(true);

  const auto wait_until_empty = [this, state, shift_register]() {
    LOG_DEBUG("Draining disinfectant");
    shift_register->write(disinfectant_out_device_id(),
                          device::digital::value::high);
    sleep_for<time_units::seconds>(1);

    while (!state->fault() && (disinfectant_level() != liquid::status::empty)) {
      // waiting...
      sleep_for<time_units::millis>(100);
      if (state->fault()) {
        return;
      }
    }

    shift_register->write(disinfectant_out_device_id(),
                          device::digital::value::low);
    LOG_DEBUG("Draining disinfectant is completed");
  };

  const auto wait_until_full = [this, state, shift_register]() {
    LOG_DEBUG("Refilling disinfectant");
    shift_register->write(disinfectant_in_device_id(),
                          device::digital::value::high);
    sleep_for<time_units::seconds>(1);

    while (!state->fault() && (disinfectant_level() != liquid::status::full)) {
      // waiting...
      sleep_for<time_units::millis>(100);
      if (state->fault()) {
        return;
      }
    }

    shift_register->write(disinfectant_in_device_id(),
                          device::digital::value::low);
    LOG_DEBUG("Refilling disinfectant is completed");
  };

  do {
    wait_until_empty();
    if (state->fault()) {
      return;
    }

    sleep_for<time_units::seconds>(10);
    if (state->fault()) {
      return;
    }
  } while (!state->fault() && (disinfectant_level() != liquid::status::empty));

  do {
    wait_until_full();
    if (state->fault()) {
      return;
    }

    sleep_for<time_units::seconds>(10);
    if (state->fault()) {
      return;
    }
  } while (!state->fault() && (disinfectant_level() != liquid::status::full));

  state->disinfectant_refilling_running(false);

  LOG_INFO("Exchanging disinfectant is finished");
}
}  // namespace impl
}  // namespace mechanism

NAMESPACE_END
