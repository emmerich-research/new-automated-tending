#include "core.hpp"

#include "config.hpp"

#include <utility>

NAMESPACE_BEGIN

// impl::ConfigImpl* Config::instance_ = nullptr;

namespace impl {
ConfigImpl::ConfigImpl(const std::string& config_path)
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

const ConfigImpl::path_container& ConfigImpl::path() {
  if (movement_path_.empty()) {
    movement_path_ =
        find<ConfigImpl::path_container>("mechanisms", "movement", "path");
  }
  return movement_path_;
}

const ConfigImpl::coordinate& ConfigImpl::path(size_t idx) {
  const auto paths = path();
  massert(idx < paths.size(), "sanity");
  return paths[idx];
}
}  // namespace impl

NAMESPACE_END
