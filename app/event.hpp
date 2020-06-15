#ifndef APP_EVENT_HPP_
#define APP_EVENT_HPP_

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace event {
struct start {};
struct stop {};

namespace tending {}
namespace spraying {}
}  // namespace event

NAMESPACE_END

#endif  // APP_EVENT_HPP_
