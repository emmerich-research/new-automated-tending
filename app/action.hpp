#ifndef APP_ACTION_HPP_
#define APP_ACTION_HPP_

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace action {
struct do_start {
  template <typename Event,
            typename FSM,
            typename SourceState,
            typename TargetState>
  void operator()(Event const&, FSM&, SourceState&, TargetState&) const;
};

struct do_stop {
  template <typename Event,
            typename FSM,
            typename SourceState,
            typename TargetState>
  void operator()(Event const&, FSM&, SourceState&, TargetState&) const;
};
}  // namespace action

NAMESPACE_END

#endif  // APP_ACTION_HPP_
