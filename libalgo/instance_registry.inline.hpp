#ifndef LIB_ALGO_INSTANCE_REGISTRY_INLINE_HPP_
#define LIB_ALGO_INSTANCE_REGISTRY_INLINE_HPP_

/** @file instance_registry.inline.hpp
 *  @brief Instance registry class implementation
 */

#include "instance_registry.hpp"

#include <utility>

NAMESPACE_BEGIN

namespace algo {
namespace impl {

template <typename T>
InstanceRegistryImpl<T>::InstanceRegistryImpl() {}

template <typename T>
template <typename... Args>
inline ATM_STATUS InstanceRegistryImpl<T>::create(const std::string& id,
                                                  Args&&... args) {
  return ATM_OK;
}
}  // namespace impl
}  // namespace algo

NAMESPACE_END

#endif  // LIB_ALGO_INSTANCE_REGISTRY_INLINE_HPP_
