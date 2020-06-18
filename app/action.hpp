#ifndef APP_ACTION_HPP_
#define APP_ACTION_HPP_

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace action {
void shutdown_hook();

struct start : public StackObj {
  template <typename Event,
            typename FSM,
            typename SourceState,
            typename TargetState>
  void operator()(Event const&, FSM&, SourceState&, TargetState&) const;
};

struct stop : public StackObj {
  template <typename Event,
            typename FSM,
            typename SourceState,
            typename TargetState>
  void operator()(Event const&, FSM&, SourceState&, TargetState&) const;
};

struct homing : public StackObj {
  template <typename Event,
            typename FSM,
            typename SourceState,
            typename TargetState>
  void operator()(Event const&, FSM&, SourceState&, TargetState&) const;

  void act() const;
};

namespace spraying {
struct job : public StackObj {
  template <typename Event,
            typename FSM,
            typename SourceState,
            typename TargetState>
  void operator()(Event const&, FSM&, SourceState&, TargetState&) const;
};

struct complete : public StackObj {
  template <typename Event,
            typename FSM,
            typename SourceState,
            typename TargetState>
  void operator()(Event const&, FSM&, SourceState&, TargetState&);
};
}  // namespace spraying

namespace tending {
struct job : public StackObj {
  template <typename Event,
            typename FSM,
            typename SourceState,
            typename TargetState>
  void operator()(Event const&, FSM&, SourceState&, TargetState&) const;
};

struct complete : public StackObj {
  template <typename Event,
            typename FSM,
            typename SourceState,
            typename TargetState>
  void operator()(Event const&, FSM&, SourceState&, TargetState&);
};
}  // namespace tending
}  // namespace action

NAMESPACE_END

#endif  // APP_ACTION_HPP_
