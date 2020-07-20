#ifndef LIB_MACHINE_ACTION_HPP_
#define LIB_MACHINE_ACTION_HPP_

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace action {

void shutdown_hook();

struct base : public StackObj {
  virtual void act() = 0;
};

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

struct fault : public StackObj {
  template <typename Event,
            typename FSM,
            typename SourceState,
            typename TargetState>
  void operator()(Event const&, FSM&, SourceState&, TargetState&) const;
};

struct restart : public StackObj {
  template <typename Event,
            typename FSM,
            typename SourceState,
            typename TargetState>
  void operator()(Event const&, FSM&, SourceState&, TargetState&) const;
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

namespace cleaning {
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
}  // namespace cleaning
}  // namespace action

NAMESPACE_END

#endif  // LIB_MACHINE_ACTION_HPP_
