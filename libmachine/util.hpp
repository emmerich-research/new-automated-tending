#ifndef LIB_MACHINE_UTIL_HPP_
#define LIB_MACHINE_UTIL_HPP_

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace machine {
namespace util {
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
}  // namespace util
}  // namespace machine

NAMESPACE_END

#endif  // LIB_MACHINE_UTIL_HPP_
