#ifndef LIB_MACHINE_UTIL_HPP_
#define LIB_MACHINE_UTIL_HPP_

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace machine {
namespace util {
/**
 * Prepare state for task execution
 */
void prepare_execution_state();

/**
 * Will reset task state and relay output to PLC
 *
 * Only call if you have initialize all core and device functionality
 */
void reset_task_state();

/**
 * Will reset ready state to true
 *
 * Only call if you have initialize all core and device functionality
 */
void reset_task_ready();

/**
 * Reset spraying
 */
void reset_spraying();

/**
 * Reset tending
 */
void reset_tending();

/**
 * Reset cleaning
 */
void reset_cleaning();

/**
 * Trigger spraying ready for both UI and Shift Register
 */
void spraying_ready();

/**
 * Trigger tending ready for both UI and Shift Register
 */
void tending_ready();

/**
 * Trigger cleaning ready for both UI and Shift Register
 */
void cleaning_ready();
}  // namespace util
}  // namespace machine

NAMESPACE_END

#endif  // LIB_MACHINE_UTIL_HPP_
