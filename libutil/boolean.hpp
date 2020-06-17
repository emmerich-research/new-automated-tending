#ifndef LIB_UTIL_BOOLEAN_HPP_
#define LIB_UTIL_BOOLEAN_HPP_

/** @file boolean.hpp
 *  @brief Boolean helper definitions
 */
namespace util {
template <typename... Args>
static bool or_(const Args&&... values) {
  auto loop = [&](auto&& value) {
    if (value) {
      throw true;
    }
  };

  try {
    (loop(values), ...);
    return false;
  } catch (bool& return_val) {
    return return_val;
  }
}

template <typename... Args>
static bool and_(const Args&&... values) {
  auto loop = [&](auto&& value) {
    if (!value) {
      throw false;
    }
  };

  try {
    (loop(values), ...);
    return true;
  } catch (bool& return_val) {
    return return_val;
  }
}
}  // namespace util

#endif  // LIB_UTIL_BOOLEAN_HPP_
