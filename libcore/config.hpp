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

#include <yaml-cpp/yaml.h>

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
 * Machine's configuration that contains all the information the manchine needed
 *
 * @author Ray Andrew
 * @date   April 2020
 */
class ConfigImpl : public StackObj {
  template <class ConfigImpl>
  template <typename... Args>
  friend ATM_STATUS StaticObj<ConfigImpl>::create(Args&&... args);

 public:
  template <typename Key>
  inline const YAML::Node operator[](const Key& key) const {
    return config_->operator[](key);
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

 private:
  /**
   * YAML::Node container that will be used as configuration
   * through all files in this
   */
  std::shared_ptr<YAML::Node> config_;

  /**
   * Config file
   */
  const std::string config_path_;
};
}  // namespace impl

NAMESPACE_END

#endif
