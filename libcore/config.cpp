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

const ConfigImpl::coordinate& ConfigImpl::spraying_position() {
  if (util::pair_empty(spraying_position_)) {
    spraying_position_ =
        find<ConfigImpl::coordinate>("mechanisms", "spraying", "position");
  }
  return spraying_position_;
}

const ConfigImpl::coordinate& ConfigImpl::tending_position() {
  if (util::pair_empty(tending_position_)) {
    tending_position_ =
        find<ConfigImpl::coordinate>("mechanisms", "tending", "position");
  }
  return tending_position_;
}

const ConfigImpl::path_container& ConfigImpl::spraying_path() {
  if (spraying_path_.empty()) {
    spraying_path_ =
        find<ConfigImpl::path_container>("mechanisms", "spraying", "path");
  }
  return spraying_path_;
}

const ConfigImpl::path_container& ConfigImpl::tending_path_edge() {
  if (tending_path_edge_.empty()) {
    tending_path_edge_ = find<ConfigImpl::path_container>(
        "mechanisms", "tending", "path", "edge");
  }
  return tending_path_edge_;
}

const ConfigImpl::path_container& ConfigImpl::tending_path_zigzag() {
  if (tending_path_zigzag_.empty()) {
    tending_path_zigzag_ = find<ConfigImpl::path_container>(
        "mechanisms", "tending", "path", "zigzag");
  }
  return tending_path_zigzag_;
}

const ConfigImpl::coordinate& ConfigImpl::spraying_path(size_t idx) {
  const auto paths = spraying_path();
  massert(idx < paths.size(), "sanity");
  return paths[idx];
}

const ConfigImpl::coordinate& ConfigImpl::tending_path_edge(size_t idx) {
  const auto paths = tending_path_edge();
  massert(idx < paths.size(), "sanity");
  return paths[idx];
}

const ConfigImpl::coordinate& ConfigImpl::tending_path_zigzag(size_t idx) {
  const auto paths = tending_path_zigzag();
  massert(idx < paths.size(), "sanity");
  return paths[idx];
}
}  // namespace impl

NAMESPACE_END
