#ifndef APP_EVENT_HPP_
#define APP_EVENT_HPP_

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace event {
struct start {};
struct stop {};

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

#endif  // APP_EVENT_HPP_
