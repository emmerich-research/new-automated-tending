#ifndef LIB_UTIL_TIMER_HPP_
#define LIB_UTIL_TIMER_HPP_

/** @file timer.hpp
 *  @brief Timer helper definitions
 */

#include <cstdlib>

using time_unit = uint64_t;

enum class time_units {
  millis,
  micros,
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

/**
 * @brief Sleep for relative time from now
 *
 * @tparam TimeUnits time units can be millis, micros, or nanos
 *
 * @param  time sleep time
 */
template <time_units TimeUnits>
void sleep_for(time_unit time);

/**
 * @brief Sleep for absolute time from given start time
 *
 * @tparam TimeUnits time units can be millis, micros, or nanos
 *
 * @param  time        delay time
 * @param  start_time  starting time, if zero it means now() + time
 */
template <time_units TimeUnits>
void sleep_until(time_unit time, time_unit start_time = 0);

#endif  // LIB_CORE_TIMER_HPP_
