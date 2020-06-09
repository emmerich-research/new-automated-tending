#include "core.hpp"

#include "config.hpp"

#include <utility>

NAMESPACE_BEGIN

// impl::ConfigImpl* Config::instance_ = nullptr;

namespace impl {
ConfigImpl::ConfigImpl(const std::string config_path)
    : config_path_{std::move(config_path)},
      config_{std::move(toml::parse(config_path))} {
  DEBUG_ONLY(obj_name_ = "ConfigImpl");
}

std::string ConfigImpl::name() const {
  if (config().contains("general") && config().at("general").contains("name")) {
    return find<std::string>("general", "name");
  }

  return "Emmerich";
}

bool ConfigImpl::debug() const {
  if (config().contains("general") &&
      config().at("general").contains("debug")) {
    return find<bool>("general", "debug");
  }

  return false;
}

std::string ConfigImpl::stepper_type() const {
  if (config().contains("devices") &&
      config().at("devices").contains("stepper")) {
    return find<std::string>("devices", "stepper", "type");
  }

  return "A4988";
}
}  // namespace impl

NAMESPACE_END
