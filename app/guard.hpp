#ifndef APP_GUARD_HPP_
#define APP_GUARD_HPP_

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace guard {
struct spraying_height {
  template <typename FSM, typename State>
  bool operator()(FSM const& fsm, State const&) const;
};

struct tending_height {
  template <typename FSM, typename State>
  bool operator()(FSM const& fsm, State const&) const;
};
}  // namespace guard

NAMESPACE_END

#endif  // APP_GUARD_HPP_
