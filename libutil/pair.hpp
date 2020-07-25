#ifndef LIB_UTIL_PAIR_H_
#define LIB_UTIL_PAIR_H_

#include <utility>

/** @file pair.hpp
 *  @brief std::pair helper definitions
 */

namespace util {
template <class T1, class T2>
bool pair_empty(const std::pair<T1, T2>& p) {
  return p == std::pair<T1, T2>();
}
}  // namespace util

#endif  // LIB_UTIL_PAIR_H_
