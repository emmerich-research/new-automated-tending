#ifndef LIB_ALGO_INSTANCE_REGISTRY_INLINE_HPP_
#define LIB_ALGO_INSTANCE_REGISTRY_INLINE_HPP_

/** @file instance_registry.inline.hpp
 *  @brief Instance registry class implementation
 */

#include "instance_registry.hpp"

#include <utility>
#include <stdexcept>

NAMESPACE_BEGIN

namespace algo {
namespace impl {

template <typename T>
InstanceRegistryImpl<T>::InstanceRegistryImpl() {
  DEBUG_ONLY(obj_name_ = "InstanceRegistryImpl");
}

template <typename T>
template <typename... Args>
inline ATM_STATUS InstanceRegistryImpl<T>::create(const std::string& id,
                                                  Args&&... args) {
  massert(container_.count(id) == 0, "instance id is not unique");
  if (container_.count(id) == 1) {
    return ATM_ERR;
  }
  // container_[id] = std::make_shared<T>(std::forward<Args>(args)...);
  container_[id] = T::create(std::forward<Args>(args)...);
  return ATM_OK;
}

template <typename T>
inline const std::shared_ptr<T>& InstanceRegistryImpl<T>::get(
    const std::string& id) {
  try {
    return container_.at(id);
  } catch ([[maybe_unused]] const std::out_of_range& e) {
    return nullptr;
  }
}
}  // namespace impl
}  // namespace algo

NAMESPACE_END

#endif  // LIB_ALGO_INSTANCE_REGISTRY_INLINE_HPP_
