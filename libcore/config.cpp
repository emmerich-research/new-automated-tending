#include "core.hpp"

#include "config.hpp"

#include <utility>

namespace toml {
template <>
struct from<ns(config::Speed)> {
  template <typename C,
            template <typename...>
            class M,
            template <typename...>
            class A>
  static ns(config::Speed) from_toml(const basic_value<C, M, A>& v) {
    ns(config::Speed) sc;
    sc.rpm = find<double>(v, "rpm");
    sc.acceleration = find<double>(v, "acceleration");
    sc.deceleration = find<double>(v, "deceleration");
    return sc;
  }
};

template <>
struct from<ns(config::MechanismSpeed)> {
  template <typename C,
            template <typename...>
            class M,
            template <typename...>
            class A>
  static ns(config::MechanismSpeed) from_toml(const basic_value<C, M, A>& v) {
    ns(config::MechanismSpeed) msp;
    if (v.contains("x")) {
      msp.x = find<ns(config::Speed)>(v, "x");
    }
    if (v.contains("y")) {
      msp.y = find<ns(config::Speed)>(v, "y");
    }
    if (v.contains("z")) {
      msp.z = find<ns(config::Speed)>(v, "z");
    }
    if (v.contains("duty-cycle")) {
      msp.duty_cycle = find<unsigned int>(v, "duty-cycle");
    }
    return msp;
  }
};

template <>
struct from<ns(config::SpeedProfile)> {
  template <typename C,
            template <typename...>
            class M,
            template <typename...>
            class A>
  static ns(config::SpeedProfile) from_toml(const basic_value<C, M, A>& v) {
    ns(config::SpeedProfile) sp;
    sp.finger_threshold = 10;
    if (v.contains("finger") && v.at("finger").contains("threshold")) {
      sp.finger_threshold = find<double>(v, "finger", "threshold");
    }
    sp.slow = find<ns(config::MechanismSpeed)>(v, "speed", "slow");
    sp.slow = find<ns(config::MechanismSpeed)>(v, "speed", "slow");
    sp.normal = find<ns(config::MechanismSpeed)>(v, "speed", "normal");
    sp.fast = find<ns(config::MechanismSpeed)>(v, "speed", "fast");
    return sp;
  }
};
}  // namespace toml

NAMESPACE_BEGIN

// impl::ConfigImpl* Config::instance_ = nullptr;

namespace config {

Speed::Speed() {
  rpm = 0.0;
  acceleration = 0.0;
  deceleration = 0.0;
}

DEBUG_ONLY(void Speed::print(std::ostream& os) const {
  os << "[rpm: " << rpm << ", accel: " << acceleration
     << ", decel: " << deceleration << "]";
})

MechanismSpeed::MechanismSpeed() {
  duty_cycle = 0;
}

DEBUG_ONLY(void MechanismSpeed::print(std::ostream& os) const {
  os << "[x: " << x << ", y: " << y << ", z: " << z
     << ", duty_cycle: " << duty_cycle << "]";
})

SpeedProfile::SpeedProfile() {}

DEBUG_ONLY(void SpeedProfile::print(std::ostream& os) const {
  os << "[slow: " << slow;
  os << ", normal: " << normal;
  os << ", fast: " << fast << "]";
})
}  // namespace config

namespace impl {
ConfigImpl::ConfigImpl(const std::string& config_path)
    : config_{toml::parse(config_path)}, config_path_{std::move(config_path)} {
  DEBUG_ONLY(obj_name_ = "ConfigImpl");
  load_speed_profiles();
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

const ConfigImpl::cleaning_container& ConfigImpl::cleaning_stations() {
  if (cleaning_stations_.empty()) {
    cleaning_stations_ = find<ConfigImpl::cleaning_container>(
        "mechanisms", "cleaning", "stations");
  }
  return cleaning_stations_;
}

const ConfigImpl::path_container& ConfigImpl::tending_path_zigzag() {
  if (tending_path_zigzag_.empty()) {
    tending_path_zigzag_ = find<ConfigImpl::path_container>(
        "mechanisms", "tending", "path", "zigzag");
  }
  return tending_path_zigzag_;
}

const ConfigImpl::coordinate ConfigImpl::spraying_path(size_t idx) {
  const auto paths = spraying_path();
  massert(idx < paths.size(), "sanity");
  return paths[idx];
}

const ConfigImpl::coordinate ConfigImpl::tending_path_edge(size_t idx) {
  const auto paths = tending_path_edge();
  massert(idx < paths.size(), "sanity");
  return paths[idx];
}

const ConfigImpl::coordinate ConfigImpl::tending_path_zigzag(size_t idx) {
  const auto paths = tending_path_zigzag();
  massert(idx < paths.size(), "sanity");
  return paths[idx];
}

const ConfigImpl::cleaning& ConfigImpl::cleaning_station(size_t idx) {
  const auto& cleanings = cleaning_stations();
  massert(idx < cleanings.size(), "sanity");
  return cleanings[idx];
}

void ConfigImpl::load_speed_profiles() {
  // Fault speed profile
  fault_speed_profile_ =
      find<config::SpeedProfile>("mechanisms", "fault", "manual");
  DEBUG_ONLY(std::cout << "Fault Speed Profile " << fault_speed_profile_
                       << std::endl);

  // Homing speed profile
  homing_speed_profile_ = find<config::SpeedProfile>("mechanisms", "homing");
  DEBUG_ONLY(std::cout << "Homing Speed Profile " << homing_speed_profile_
                       << std::endl);

  // Spraying speed profile
  spraying_speed_profile_ =
      find<config::SpeedProfile>("mechanisms", "spraying");
  DEBUG_ONLY(std::cout << "Spraying Speed Profile " << spraying_speed_profile_
                       << std::endl);

  // Tending speed profile
  tending_speed_profile_ = find<config::SpeedProfile>("mechanisms", "tending");
  DEBUG_ONLY(std::cout << "Tending Speed Profile " << tending_speed_profile_
                       << std::endl);

  // Cleaning speed profile
  cleaning_speed_profile_ =
      find<config::SpeedProfile>("mechanisms", "cleaning");
  DEBUG_ONLY(std::cout << "Cleaning Speed Profile " << cleaning_speed_profile_
                       << std::endl);
}

const config::MechanismSpeed& ConfigImpl::fault_speed_profile(
    const config::speed& speed_profile) const {
  if (speed_profile == config::speed::slow) {
    return fault_speed_profile_.slow;
  } else if (speed_profile == config::speed::normal) {
    return fault_speed_profile_.normal;
  } else {
    return fault_speed_profile_.fast;
  }
}

const config::MechanismSpeed& ConfigImpl::homing_speed_profile(
    const config::speed& speed_profile) const {
  if (speed_profile == config::speed::slow) {
    return homing_speed_profile_.slow;
  } else if (speed_profile == config::speed::normal) {
    return homing_speed_profile_.normal;
  } else {
    return homing_speed_profile_.fast;
  }
}

const config::SpeedProfile& ConfigImpl::homing_speed_profile() const {
  return homing_speed_profile_;
}

const config::MechanismSpeed& ConfigImpl::spraying_speed_profile(
    const config::speed& speed_profile) const {
  if (speed_profile == config::speed::slow) {
    return spraying_speed_profile_.slow;
  } else if (speed_profile == config::speed::normal) {
    return spraying_speed_profile_.normal;
  } else {
    return spraying_speed_profile_.fast;
  }
}

const config::MechanismSpeed& ConfigImpl::tending_speed_profile(
    const config::speed& speed_profile) const {
  if (speed_profile == config::speed::slow) {
    return tending_speed_profile_.slow;
  } else if (speed_profile == config::speed::normal) {
    return tending_speed_profile_.normal;
  } else {
    return tending_speed_profile_.fast;
  }
}

const config::MechanismSpeed& ConfigImpl::cleaning_speed_profile(
    const config::speed& speed) const {
  if (speed == config::speed::slow) {
    return cleaning_speed_profile_.slow;
  } else if (speed == config::speed::normal) {
    return cleaning_speed_profile_.normal;
  } else {
    return cleaning_speed_profile_.fast;
  }
}
}  // namespace impl

NAMESPACE_END
