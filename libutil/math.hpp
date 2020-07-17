#ifndef LIB_UTIL_MATH_HPP_
#define LIB_UTIL_MATH_HPP_

#include <type_traits>

/** @file math.hpp
 *  @brief Math helper
 */

namespace util {
namespace math {
// http://en.wikipedia.org/wiki/Moving_average#Exponential_moving_average
// credits for @steveha
// for his answer at
// https://stackoverflow.com/a/10990656
template <unsigned int N,
          typename T = unsigned int,
          typename U = double,
          typename = std::enable_if_t<std::is_integral_v<T> &&
                                      std::is_floating_point_v<U>>>
void moving_average(U& avg, const T& input) {
  static constexpr U alpha = 1.0 / static_cast<U>(N);
  static constexpr U mult = 1.0 - alpha;
  static_assert(alpha > 0.0 && alpha < 1.0,
                "1 / N must be more than 0 and less than 1");
  avg = alpha * input + mult * avg;
}
}  // namespace math
}  // namespace util

#endif  // LIB_UTIL_MATH_HPP_
