#ifndef LIB_CORE_CONFIG_HPP_
#define LIB_CORE_CONFIG_HPP_

/** @file config.hpp
 *  @brief Config singleton class definition
 *
 * Project's configuration
 */

#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <toml.hpp>

#include <libutil/util.hpp>

#include "common.hpp"

#include "allocation.hpp"

NAMESPACE_BEGIN

// forward declarations
namespace config {
struct Speed;
struct MechanismSpeed;
struct SpeedProfile;
}  // namespace config

namespace impl {
class ConfigImpl;
}  // namespace impl

/** impl::ConfigImpl singleton class using StaticObj */
using Config = StaticObj<impl::ConfigImpl>;

namespace config {
/**
 * @brief Speed implementation.
 *
 * Struct for speed profile for motor configuration
 *
 * @author Ray Andrew
 * @date   June 2020
 */
struct Speed {
  Speed();
  DEBUG_ONLY_DEFINITION(void print(std::ostream& os) const);

  double rpm;
  double acceleration;
  double deceleration;
};

/**
 * @brief MechanismSpeedProfile implementation.
 *
 * Struct for speed profile for mechanism configuration
 *
 * @author Ray Andrew
 * @date   June 2020
 */
struct MechanismSpeed {
  MechanismSpeed();
  DEBUG_ONLY_DEFINITION(void print(std::ostream& os) const);

  Speed        x;
  Speed        y;
  Speed        z;
  unsigned int duty_cycle;
};

/**
 * @brief SpeedProfile implementation.
 *
 * Struct for multiple speed profile configuration
 *
 * @author Ray Andrew
 * @date   June 2020
 */
struct SpeedProfile {
  SpeedProfile();
  DEBUG_ONLY_DEFINITION(void print(std::ostream& os) const);

  MechanismSpeed slow;
  MechanismSpeed normal;
  MechanismSpeed fast;
};

enum class speed { slow, normal, fast };
}  // namespace config

template <class T>
auto operator<<(std::ostream& os, T const& t) -> decltype(t.print(os), os) {
  t.print(os);
  return os;
}

namespace impl {
/**
 * @brief Config implementation.
 *        This is a class wrapper that should not be instantiated and accessed
 * publicly.
 *
 * Machine's configuration that contains all the information the machine needed
 *
 * @author Ray Andrew
 * @date   April 2020
 */
class ConfigImpl : public StackObj {
  template <class ConfigImpl>
  template <typename... Args>
  friend ATM_STATUS StaticObj<ConfigImpl>::create(Args&&... args);

 public:
  typedef std::pair<double, double>                      coordinate;
  typedef std::vector<coordinate>                        path_container;
  typedef std::tuple<double, double, unsigned int, bool> cleaning;
  typedef std::vector<cleaning>                          cleaning_container;
  /**
   * Get name of app from config
   *
   * It should be in key "general.app"
   *
   * @return application name
   */
  std::string name() const;
  /**
   * Get debug status of logging message
   *
   * It should be in key "general.debug"
   *
   * @return debug status
   */
  bool debug() const;
  /**
   * Get task timeout
   *
   * It should be in key "mechanism.fault.timeout"

   * @return task timeout
   */
  unsigned int timeout() const;
  /**
   * Get speed Profile of Fault mechanism
   *
   * @param speed_profile type of speed
   *
   * @return fault speed profile
   */
  const config::MechanismSpeed& fault_speed_profile(
      const config::speed& speed_profile) const;
  /**
   * Get speed Profile of Homing mechanism
   *
   * @return homing speed profile
   */
  const config::SpeedProfile& homing_speed_profile() const;
  /**
   * Get speed Profile of Homing mechanism
   *
   * @param speed_profile type of speed
   *
   * @return homing speed profile
   */
  const config::MechanismSpeed& homing_speed_profile(
      const config::speed& speed_profile) const;
  /**
   * Get speed Profile of Spraying mechanism
   *
   * @param speed_profile type of speed
   *
   * @return spraying speed profile
   */
  const config::MechanismSpeed& spraying_speed_profile(
      const config::speed& speed_profile) const;
  /**
   * Get speed Profile of Tending mechanism
   *
   * @param speed_profile type of speed
   *
   * @return spraying speed profile
   */
  const config::MechanismSpeed& tending_speed_profile(
      const config::speed& speed_profile) const;
  /**
   * Get speed Profile of Cleaning mechanism
   *
   * @param speed_profile type of speed
   *
   * @return cleaning speed profile
   */
  const config::MechanismSpeed& cleaning_speed_profile(
      const config::speed& speed_profile) const;
  /**
   * Get stepper x-axis device info
   *
   * It should be in key "devices.stepper.x"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return stepper x-axis info with type T
   */
  template <typename T, typename... Keys>
  inline T stepper_x(Keys&&... keys) const {
    return find<T>("devices", "stepper", "x", std::forward<Keys>(keys)...);
  }
  /**
   * Get stepper y-axis device info
   *
   * It should be in key "devices.stepper.y"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return stepper y-axis info with type T
   */
  template <typename T, typename... Keys>
  inline T stepper_y(Keys&&... keys) const {
    return find<T>("devices", "stepper", "y", std::forward<Keys>(keys)...);
  }
  /**
   * Get stepper z-axis device info
   *
   * It should be in key "devices.stepper.z"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return stepper z-axis info with type T
   */
  template <typename T, typename... Keys>
  inline T stepper_z(Keys&&... keys) const {
    return find<T>("devices", "stepper", "z", std::forward<Keys>(keys)...);
  }
  /**
   * Get limit switch x-axis device info
   *
   * It should be in key "devices.limit-switch.x"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return limit switch x-axis info with type T
   */
  template <typename T, typename... Keys>
  inline T limit_switch_x(Keys&&... keys) const {
    return find<T>("devices", "limit-switch", "x", std::forward<Keys>(keys)...);
  }
  /**
   * Get limit switch y-axis device info
   *
   * It should be in key "devices.limit-switch.y"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return limit switch y-axis info with type T
   */
  template <typename T, typename... Keys>
  inline T limit_switch_y(Keys&&... keys) const {
    return find<T>("devices", "limit-switch", "y", std::forward<Keys>(keys)...);
  }
  /**
   * Get limit switch upper bound z-axis device info
   *
   * It should be in key "devices.limit-switch.z1"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return limit switch upper bound z-axis info with type T
   */
  template <typename T, typename... Keys>
  inline T limit_switch_z1(Keys&&... keys) const {
    return find<T>("devices", "limit-switch", "z1",
                   std::forward<Keys>(keys)...);
  }
  /**
   * Get limit switch lower bound z-axis device info
   *
   * It should be in key "devices.limit-switch.z2"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return limit switch lower bound z-axis info with type T
   */
  template <typename T, typename... Keys>
  inline T limit_switch_z2(Keys&&... keys) const {
    return find<T>("devices", "limit-switch", "z2",
                   std::forward<Keys>(keys)...);
  }
  /**
   * Get limit switch finger protection device info
   *
   * It should be in key "devices.limit-switch.finger-protection"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return limit switch finger protection info with type T
   */
  template <typename T, typename... Keys>
  inline T limit_switch_finger_protection(Keys&&... keys) const {
    return find<T>("devices", "limit-switch", "finger-protection",
                   std::forward<Keys>(keys)...);
  }
  /**
   * Get finger device info
   *
   * It should be in key "devices.finger.motor"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return finger device info with type T
   */
  template <typename T, typename... Keys>
  inline T finger(Keys&&... keys) const {
    return find<T>("devices", "finger", "motor", std::forward<Keys>(keys)...);
  }
  /**
   * Get finger infrared device info
   *
   * It should be in key "devices.finger.infrared"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return finger device info with type T
   */
  template <typename T, typename... Keys>
  inline T finger_infrared(Keys&&... keys) const {
    return find<T>("devices", "finger", "infrared",
                   std::forward<Keys>(keys)...);
  }
  /**
   * Get analog device info
   *
   * It should be in key "devices.analog"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return stepper x-axis info with type T
   */
  template <typename T, typename... Keys>
  inline T analog(Keys&&... keys) const {
    return find<T>("devices", "analog", std::forward<Keys>(keys)...);
  }
  /**
   * Get spraying position
   *
   * It should be in key "mechanisms.spraying.position"
   *
   * @return spraying position
   */
  const coordinate& spraying_position();
  /**
   * Get spraying movement path
   *
   * It should be in key "mechanisms.spraying.path"
   *
   * @return spraying movement path
   */
  const path_container& spraying_path();
  /**
   * Get spraying movement path coordinate at specified index
   *
   * @param idx index to get
   *
   * @return spraying movement path at specified index
   */
  const coordinate spraying_path(size_t idx);
  /**
   * Get tending position
   *
   * It should be in key "mechanisms.tending.position"
   *
   * @return tending position
   */
  const coordinate& tending_position();
  /**
   * Get tending edge movement path
   *
   * It should be in key "mechanisms.tending.path.edge"
   *
   * @return tending edge movement path
   */
  const path_container& tending_path_edge();
  /**
   * Get tending edge movement path coordinate at specified index
   *
   * @param idx index to get
   *
   * @return tending edge movement path at specified index
   */
  const coordinate tending_path_edge(size_t idx);
  /**
   * Get tending zigzag movement path
   *
   * It should be in key "mechanisms.tending.path.zigzag"
   *
   * @return tending zigzag movement path
   */
  const path_container& tending_path_zigzag();
  /**
   * Get tending zigzag movement path coordinate at specified index
   *
   * @param idx index to get
   *
   * @return tending movement path at specified index
   */
  const coordinate tending_path_zigzag(size_t idx);
  /**
   * Get cleaning stations
   *
   * It should be in key "mechanisms.cleaning.stations"
   *
   * @return tending zigzag movement path
   */
  const cleaning_container& cleaning_stations();
  /**
   * Get cleaning station at specified index
   *
   * @param idx index to get
   *
   * @return cleaning station at specified index
   */
  const cleaning& cleaning_station(size_t idx);
  /**
   * Get mechanisms fault manual mode movement
   *
   * It should be in key "mechanisms.fault.manual.movement"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return manual mode config
   */
  template <typename T, typename... Keys>
  inline T fault_manual_movement(Keys&&... keys) const {
    return find<T>("mechanisms", "fault", "manual", "movement",
                   std::forward<Keys>(keys)...);
  }
  /**
   * Get shift register device configuration
   *
   * It should be in key "devices.shift-register"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return shift register device configuration
   */
  template <typename T, typename... Keys>
  inline T shift_register(Keys&&... keys) const {
    return find<T>("devices", "shift-register", std::forward<Keys>(keys)...);
  }
  /**
   * Get communication device from PLC to RaspberryPI
   *
   * It should be in key "devices.plc-to-pi"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return communication device info from PLC to RaspberryPI with type T
   */
  template <typename T, typename... Keys>
  inline T plc_to_pi(Keys&&... keys) const {
    return find<T>("devices", "plc-to-pi", std::forward<Keys>(keys)...);
  }
  /**
   * Get ultrasonic device
   *
   * It should be in key "devices.ultrasonic"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return ultrasonic device
   */
  template <typename T, typename... Keys>
  inline T ultrasonic(Keys&&... keys) const {
    return find<T>("devices", "ultrasonic", std::forward<Keys>(keys)...);
  }
  /**
   * Get float sensor device
   *
   * It should be in key "devices.float-sensor"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return float sensor device
   */
  template <typename T, typename... Keys>
  inline T float_sensor(Keys&&... keys) const {
    return find<T>("devices", "float-sensor", std::forward<Keys>(keys)...);
  }
  /**
   * Get liquid refilling config
   *
   * It should be in key "mechanisms.liquid-refilling"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return liquid refilling mechanisms
   */
  template <typename T, typename... Keys>
  inline T liquid_refilling(Keys&&... keys) const {
    return find<T>("mechanisms", "liquid-refilling",
                   std::forward<Keys>(keys)...);
  }
  /**
   * Get sonicator relay device
   *
   * It should be in key "devices.sonicator-relay"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return sonicator-relay device
   */
  template <typename T, typename... Keys>
  inline T sonicator_relay(Keys&&... keys) const {
    return find<T>("devices", "sonicator-relay", std::forward<Keys>(keys)...);
  }

 private:
  /**
   * ConfigImpl Constructor
   *
   * Initialize YAML-CPP and load yaml config file for this project
   *
   * @param config_path   config file path
   */
  explicit ConfigImpl(const std::string& config_path);
  /**
   * ConfigImpl Destructor
   *
   * Noop
   *
   */
  ~ConfigImpl() = default;
  /**
   * Get TOML Config
   *
   * @return config tree
   */
  inline const toml::value& config() const { return config_; }
  /**
   * Find key in the TOML config
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return config value with type T
   */
  template <typename T, typename... Keys>
  inline T find(Keys&&... keys) const {
    return toml::find<T>(config(), std::forward<Keys>(keys)...);
    }
    /**
     * Load speed profile for all mechanisms
     */
    void load_speed_profiles();

   private:
    /**
     * TOML config data
     */
    const toml::value config_;
    /**
     * Config file
     */
    const std::string config_path_;
    /**
     * Spraying movement path
     */
    path_container spraying_path_;
    /**
     * Spraying position
     */
    coordinate spraying_position_;
    /**
     * Tending movement path for edge pattern
     */
    path_container tending_path_edge_;
    /**
     * Tending movement path for zigzag pattern
     */
    path_container tending_path_zigzag_;
    /**
     * Tending position
     */
    coordinate tending_position_;
    /**
     * Cleaning container
     */
    cleaning_container cleaning_stations_;
    /**
     * Fault speed profile
     */
    config::SpeedProfile fault_speed_profile_;
    /**
     * Homing speed profile
     */
    config::SpeedProfile homing_speed_profile_;
    /**
     * Tending speed profile
     */
    config::SpeedProfile tending_speed_profile_;
    /**
     * Spraying speed profile
     */
    config::SpeedProfile spraying_speed_profile_;
    /**
     * Cleaning speed profile
     */
    config::SpeedProfile cleaning_speed_profile_;
  };
}  // namespace impl

NAMESPACE_END

#endif
