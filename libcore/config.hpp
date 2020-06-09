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

#include <toml.hpp>

#include "common.hpp"

#include "allocation.hpp"
#include "macros.hpp"

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
   * Get limit switch z-axis device info
   *
   * It should be in key "devices.limit-switch.z"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return limit switch z-axis info with type T
   */
  template <typename T, typename... Keys>
  inline T limit_switch_z(Keys&&... keys) const {
    return find<T>("devices", "limit-switch", "z", std::forward<Keys>(keys)...);
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

 private:
  /**
   * ConfigImpl Constructor
   *
   * Initialize YAML-CPP and load yaml config file for this project
   *
   * @param config_path   config file path
   */
  explicit ConfigImpl(const std::string config_path);
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
