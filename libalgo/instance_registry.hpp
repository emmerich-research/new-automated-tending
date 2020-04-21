#ifndef LIB_ALGO_INSTANCE_REGISTRY_HPP_
#define LIB_ALGO_INSTANCE_REGISTRY_HPP_

/** @file instance_registry.hpp
 *  @brief Instance registry class definition
 *
 * Singleton registry class to hold specific class instances
 */

#include <memory>
#include <string>
#include <unordered_map>

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace algo {
namespace impl {
template <typename T>
class InstanceRegistryImpl;
}

/** impl::InstanceRegistryImpl singleton class using StaticObj */
template <typename T>
using InstanceRegistry = StaticObj<impl::InstanceRegistryImpl<T>>;

namespace impl {
/**
 * @brief Instance Registry implementation.
 *        This is a class wrapper that should not be instantiated and accessed
 * publicly.
 *
 * Instance registry will create, hold, and destroy all the instances through
 * the lifetime of application
 *
 * @author Ray Andrew
 * @date   April 2020
 */
template <typename T>
class InstanceRegistryImpl : public StackObj {
  template <typename... Args>
  friend ATM_STATUS InstanceRegistry<T>::create(Args&&... args);

 public:
  /**
   * Create new instance of T
   *
   * @tparam Args  variadic template for arguments
   *
   * @param  id    unique identifier of instance
   * @param  args  arguments to pass to constructor of T
   */
  template <typename... Args>
  inline ATM_STATUS create(const std::string& id, Args&&... args);
  /**
   * Get instance of T with unique id
   *
   * @param  id    unique identifier of instance
   */
  inline const std::shared_ptr<T>& get(const std::string& id);

 private:
  /**
   * InstanceRegistryImpl Constructor
   */
  explicit InstanceRegistryImpl();
  /**
   * InstanceRegistryImpl Destructor
   *
   * Noop
   */
  ~InstanceRegistryImpl() = default;

 private:
  /**
   * Instances container
   */
  std::unordered_map<std::string, std::shared_ptr<T>> container_;
};
}  // namespace impl
}  // namespace algo

NAMESPACE_END

#endif  // LIB_ALGO_INSTANCE_REGISTRY_HPP_
