#include "machine.hpp"

#include "event.hpp"

#include <afsm/fsm.hpp>

#include <type_traits>

namespace afsm {
template <>
struct event_priority_traits<ns(event::fault::trigger)>
    : std::integral_constant<event_priority_type, 100500> {};

template <>
struct event_priority_traits<ns(event::fault::manual)>
    : std::integral_constant<event_priority_type, 100400> {};

template <>
struct event_priority_traits<ns(event::fault::restart)>
    : std::integral_constant<event_priority_type, 100300> {};
} /* namespace afsm */

NAMESPACE_BEGIN

namespace event {}

NAMESPACE_END
