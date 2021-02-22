#ifndef LIB_UTIL_TIME_HPP_
#define LIB_UTIL_TIME_HPP_

/** @file time.hpp
 *  @brief Time helper definitions
 */

#include <chrono>
#include <exception>
#include <string>

using Clock = std::chrono::system_clock;
using TimePoint = Clock::time_point;

#endif  // LIB_UTIL_TIME_HPP_
