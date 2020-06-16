#ifndef LIB_CORE_ALLOCATION_INLINE_HPP_
#define LIB_CORE_ALLOCATION_INLINE_HPP_

/** @file allocation.inline.hpp
 *  @brief Allocation template class implementation
 */

#include "allocation.hpp"

#include <utility>

#include <libutil/util.hpp>

NAMESPACE_BEGIN

#ifdef NDEBUG

inline const std::string& AllocObj::obj_name() const {
  return obj_name_;
}

template <typename OStream>
OStream& operator<<(OStream& os, const AllocObj& obj) {
  return os << obj.obj_name();
}

#endif  // NDEBUG

template <typename T>
T* StaticObj<T>::instance_ = nullptr;

template <typename T>
template <typename... Args>
inline ATM_STATUS StaticObj<T>::create(Args&&... args) {
  massert(instance_ == nullptr, "create only can be called once");
  if (instance_ == nullptr) {
    static T instance(std::forward<Args>(args)...);
    instance_ = &instance;
  }
  if (instance_ == nullptr) {
    return ATM_ERR;
  } else {
    massert(instance_ != nullptr, "sanity check");
    return ATM_OK;
  }
}

template <typename T>
inline T* StaticObj<T>::get() {
  massert(instance_ != nullptr, "can only be called if it is initialized");
  return instance_;
}

NAMESPACE_END

#endif  // LIB_CORE_ALLOCATION_INLINE_HPP_
