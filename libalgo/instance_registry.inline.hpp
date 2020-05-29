#ifndef LIB_ALGO_INSTANCE_REGISTRY_INLINE_HPP_
#define LIB_ALGO_INSTANCE_REGISTRY_INLINE_HPP_

#include "instance_registry.hpp"

#include <stdexcept>
#include <type_traits>
#include <utility>

NAMESPACE_BEGIN

namespace algo {
namespace impl {

template <typename T>
InstanceRegistryImpl<T>::InstanceRegistryImpl() {
  DEBUG_ONLY(obj_name_ = "InstanceRegistryImpl");
}

template <typename T>
template <typename U, typename... Args, typename>
inline ATM_STATUS InstanceRegistryImpl<T>::create(const std::string& id,
                                                  Args&&... args) {
  massert(container_.count(id) == 0, "instance id is not unique");
  if (container_.count(id) == 1) {
    return ATM_ERR;
  }
  LOG_DEBUG("InstanceRegistryImpl::create instance with key {}", id);
  container_[id] = U::create(std::forward<Args>(args)...);
  return ATM_OK;
}

template <typename T>
template <typename U, typename... Args, typename>
inline ATM_STATUS InstanceRegistryImpl<T>::create(
    const std::map<const std::string&, Args&&...>& initializers) {
  for (const auto& [id, args] : initializers) {
    ATM_STATUS res = create(id, std::forward<Args>(args)...);
    if (res == ATM_ERR) {
      LOG_DEBUG("Failed to create instances with id = {}", id);
      return ATM_ERR;
    }
  }

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
