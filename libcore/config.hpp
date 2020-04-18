#ifndef LIB_CORE_CONFIG_HPP_
#define LIB_CORE_CONFIG_HPP_

/** \file config.hpp
 *  \brief Config singleton class definition
 *
 * Project's configuration
 */

#include <memory>
#include <string>

#include <yaml-cpp/yaml.h>

#include "common.hpp"

#include "allocation.hpp"
#include "macros.hpp"

NAMESPACE_BEGIN

namespace impl {
class ConfigImpl;
}

/**
 * @brief Singleton of impl::StateImpl class
 *
 * Public and singleton class of impl::StateImpl class
 *
 * @author Ray Andrew
 * @date   April 2020
 */
class Config : StaticObj {
 public:
  /**
   * Config singleton initialization
   *
   * @param  args    arguments are same with impl::ConfigImpl()
   * @return impl::ConfigImpl pointer
   */
  template <typename... Args>
  static impl::ConfigImpl* create(Args... args);

  /**
   * Get impl::ConfigImpl pointer
   *
   * @return impl::ConfigImpl pointer that has been initialized
   */
  static impl::ConfigImpl* get();

 private:
  static impl::ConfigImpl* instance_;
};

namespace impl {
/**
 * \brief Config implementation.
 *        This is a class wrapper that should not be instantiated and accessed
 * publicly.
 *
 * Machine's configuration that contains all the information the manchine needed
 *
 * @author Ray Andrew
 * @date   April 2020
 */
class ConfigImpl : public StackObj {
  template <typename... Args>
  friend ConfigImpl* Config::create(Args... args);

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

 private:
  /**
   * YAML::Node container that will be used as configuration
   * through all files in this
   */
  std::shared_ptr<YAML::Node> config_;

  /**
   * Config file
   */
  const std::string           config_path_;
};
}  // namespace impl

template <typename... Args>
inline impl::ConfigImpl* Config::create(Args... args) {
  massert(instance_ == nullptr, "create only can be called once");
  if (instance_ == nullptr) {
    static impl::ConfigImpl config(args...);
    instance_ = &config;
  }
  massert(instance_ != nullptr, "sanity check");
  return instance_;
}

inline impl::ConfigImpl* Config::get() {
  massert(instance_ != nullptr, "can only be called if it is initialized");
  return instance_;
}

NAMESPACE_END

#endif
