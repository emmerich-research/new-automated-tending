#include "mechanism.hpp"

#include "liquid-refilling.hpp"

#include <optional>

NAMESPACE_BEGIN

namespace mechanism {
namespace impl {
LiquidRefillingImpl::LiquidRefillingImpl(
    const std::string& water_level_device_id,
    const std::string& disinfectant_level_device_id)
    : active_{false},
      water_level_device_id_{water_level_device_id},
      disinfectant_level_device_id_{disinfectant_level_device_id} {
  setup_devices();
}

LiquidRefillingImpl::~LiquidRefillingImpl() {}

liquid::status LiquidRefillingImpl::water_level() const {
  massert(Config::get() != nullptr, "sanity");

  auto* config = Config::get();
  auto  water_level = water_level_device()->distance(
      config->ultrasonic<double>("water-level", "max-range"));

  if (water_level) {
    double height = *water_level;

    if (height <= 3.0) {
      return liquid::status::full;
    } else if (height >= 8.0) {
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

    if (height <= 3.0) {
      return liquid::status::full;
    } else if (height >= 6.0) {
      return liquid::status::empty;
    } else {
      return liquid::status::normal;
    }
  }

  return liquid::status::unknown;
}

void LiquidRefillingImpl::setup_devices() {
  massert(device::UltrasonicDeviceRegistry::get() != nullptr, "sanity");

  auto* ultrasonic_device_registry = device::UltrasonicDeviceRegistry::get();

  auto&& water_level_device =
      ultrasonic_device_registry->get(water_level_device_id());
  auto&& disinfectant_level_device =
      ultrasonic_device_registry->get(disinfectant_level_device_id());

  if (!water_level_device || !disinfectant_level_device) {
    active_ = false;
    return;
  }

  water_level_device_ = water_level_device;
  disinfectant_level_device_ = disinfectant_level_device;
}
}  // namespace impl
}  // namespace mechanism

NAMESPACE_END
