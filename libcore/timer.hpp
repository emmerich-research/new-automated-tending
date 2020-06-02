#ifndef LIB_CORE_TIMER_HPP_
#define LIB_CORE_TIMER_HPP_

/** @file timer.hpp
 *  @brief Timer helper definitions
 */

#include <cstdlib>

#include "common.hpp"

NAMESPACE_BEGIN

using time_unit = uint64_t;

enum class time_units {
  micros,
  millis,
  nanos,
};

/**
 * @brief Get time stamp in milliseconds.
 *
 * taken from https://stackoverflow.com/a/49066369/6808347 \n
 * credits to Gabriel Staples \n
 *
 * @return time stamp in microseconds
 */
time_unit millis(void);

/**
 * @brief Get time stamp in microseconds.
 *
 * taken from https://stackoverflow.com/a/49066369/6808347 \n
 * credits to Gabriel Staples \n
 *
 * @return time stamp in microseconds
 */
time_unit micros(void);

/**
 * @brief Get time stamp in nanoseconds.
 *
 * taken from https://stackoverflow.com/a/49066369/6808347 \n
 * credits to Gabriel Staples \n
 *
 * @return time stamp in nanoseconds
 */
time_unit nanos(void);

template<time_units TimeUnits>
void safe_delay(time_unit time);

NAMESPACE_END

#endif  // LIB_CORE_TIMER_HPP_
