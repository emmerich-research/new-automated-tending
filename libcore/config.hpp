#ifndef LIB_CORE_CONFIG_HPP_
#define LIB_CORE_CONFIG_HPP_

/** @file config.hpp
 *  @brief Config singleton class definition
 *
 * Project's configuration
 */

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <toml.hpp>

#include <libutil/util.hpp>

#include "common.hpp"

#include "allocation.hpp"

NAMESPACE_BEGIN

namespace impl {
class ConfigImpl;
}

/** impl::ConfigImpl singleton class using StaticObj */
using Config = StaticObj<impl::ConfigImpl>;

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
  typedef std::pair<double, double> coordinate;
  typedef std::vector<coordinate>   path_container;
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
   * Get stepper type
   *
   * It should be in key "devices.stepper"
   *
   * @return stepper type
   */
  std::string stepper_type() const;
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
   * Get spray device info
   *
   * It should be in key "devices.spray"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return spray device info with type T
   */
  template <typename T, typename... Keys>
  inline T spray(Keys&&... keys) const {
    return find<T>("devices", "spray", std::forward<Keys>(keys)...);
  }
  /**
   * Get finger device info
   *
   * It should be in key "devices.finger"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return finger device info with type T
   */
  template <typename T, typename... Keys>
  inline T finger(Keys&&... keys) const {
    return find<T>("devices", "finger", std::forward<Keys>(keys)...);
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
   * Get movement mechanism path
   *
   * It should be in key "mechanisms.movement.path"
   *
   * @return movement mechanism path
   */
  const path_container& path();
  /**
   * Get movement mechanism path coordinate at specified index
   *
   * @param idx index to get
   *
   * @return movement mechanism path at specified index
   */
  const coordinate& path(size_t idx);
  /**
   * Get communication device from PLC to RaspberryPI
   *
   * It should be in key "devices.communication.input"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return communication device info from PLC to RaspberryPI with type T
   */
  template <typename T, typename... Keys>
  inline T plc_to_pi(Keys&&... keys) const {
    return find<T>("devices", "communication", "input",
                   std::forward<Keys>(keys)...);
  }
  /**
   * Get communication device from RaspberryPI to PLC
   *
   * It should be in key "devices.communication.output"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return communication device info from RaspberryPI to PLC with type T
   */
  template <typename T, typename... Keys>
  inline T pi_to_plc(Keys&&... keys) const {
    return find<T>("devices", "communication", "output",
                   std::forward<Keys>(keys)...);
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

 private:
  /**
   * Movement mechanism path
   */
  path_container movement_path_;
  /**
   * TOML config data
   */
  const toml::value config_;
  /**
   * Config file
   */
  const std::string config_path_;
};
}  // namespace impl

NAMESPACE_END

#endif
