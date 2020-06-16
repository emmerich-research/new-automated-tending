#ifndef APP_EVENT_HPP_
#define APP_EVENT_HPP_

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace event {
struct start {};
struct stop {};

namespace spraying {
// get into the state
struct start {};
// preparing
struct prepare {};
// run real job
struct run {};
// finishing job
struct finish {};
}  // namespace spraying

namespace tending {
struct start {};
struct prepare {};
struct finish {};
}  // namespace tending
}  // namespace event

NAMESPACE_END

#endif  // APP_EVENT_HPP_
