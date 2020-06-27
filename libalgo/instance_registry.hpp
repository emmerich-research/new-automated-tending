#ifndef LIB_ALGO_INSTANCE_REGISTRY_HPP_
#define LIB_ALGO_INSTANCE_REGISTRY_HPP_

/** @file instance_registry.hpp
 *  @brief Instance registry class definition
 *
 * Singleton registry class to hold specific class instances
 */

#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace algo {
// forward declaration
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
 * Instance registry will create, hold, and destroy all the instances
 *
 * @tparam T any type to hold the instances
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
   * This method is only enabled if U is base of T or T itself
   *
   * @tparam U     factory class which is base of T or T itself
   * @tparam Args  variadic template for arguments
   *
   * @param  id    unique identifier of instance
   * @param  args  arguments to pass to constructor of U
   *
   * @return status ATM_ERR or ATM_OK
   */
  template <typename U = T,
            typename... Args,
            typename = std::enable_if_t<std::is_base_of<T, U>::value>>
  inline ATM_STATUS create(const std::string& id, Args&&... args);
  /**
   * Create multiple instances of T
   *
   * This method is only enabled if U is base of T or T itself
   *
   * @tparam U           factory class which is base of T or T itself
   * @tparam Args        variadic template for arguments
   *
   * @param  initializers arguments that contain id and args to pass to
   * constructor of
   * U
   */
  template <typename U = T,
            typename... Args,
            typename = std::enable_if_t<std::is_base_of<T, U>::value>>
  inline ATM_STATUS create(
      const std::map<const std::string&, Args&&...>& initializers);
  /**
   * Get instance of T with unique id
   *
   * @param  id    unique identifier of instance
   *
   * @return instance of given id or fail
   */
  inline const std::shared_ptr<T>& get(const std::string& id);
  /**
   * Check instance of T with unique id
   *
   * @param  id    unique identifier of instance
   *
   * @return exist or not
   */
  inline bool exist(const std::string& id) const;

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
  virtual ~InstanceRegistryImpl() = default;

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
