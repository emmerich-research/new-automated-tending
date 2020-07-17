#ifndef LIB_MACHINE_EVENT_HPP_
#define LIB_MACHINE_EVENT_HPP_

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace event {
struct start {};
struct stop {};

namespace fault {
struct trigger {};
struct restart {};
struct manual {};
};  // namespace fault

struct task_complete {};

namespace spraying {
struct start {};
struct run {};
}  // namespace spraying

namespace tending {
struct start {};
struct run {};
}  // namespace tending
}  // namespace event

NAMESPACE_END

#endif  // LIB_MACHINE_EVENT_HPP_
