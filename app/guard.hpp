#ifndef APP_GUARD_HPP_
#define APP_GUARD_HPP_

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace guard {
struct guard_base : public StackObj {
  virtual bool check() const = 0;
};

struct machine_ready : public StackObj {
  template <typename FSM, typename State>
  bool operator()(FSM const& fsm, State const&) const;
};

struct homing : public guard_base {
  template <typename FSM, typename State>
  bool operator()(FSM const& fsm, State const&) const;

  virtual bool check() const override;
};

struct e_stop : public guard_base {
  template <typename FSM, typename State>
  bool operator()(FSM const& fsm, State const&) const;

  virtual bool check() const override;
};

struct reset : public guard_base {
  template <typename FSM, typename State>
  bool operator()(FSM const& fsm, State const&) const;

  virtual bool check() const override;
};

namespace spraying {
struct height : public guard_base {
  template <typename FSM, typename State>
  bool operator()(FSM const& fsm, State const&) const;

  virtual bool check() const override;
};

struct completed : public StackObj {
  template <typename FSM, typename State>
  auto operator()(FSM const& fsm, State const&) const
      -> decltype(fsm.is_spraying_completed(), bool());

  template <typename FSM, typename State>
  auto operator()(FSM const& fsm, State const&) const
      -> decltype(fsm.enclosing_fsm().is_spraying_completed(), bool());
};
}  // namespace spraying

namespace tending {
struct height : public guard_base {
  template <typename FSM, typename State>
  bool operator()(FSM const& fsm, State const&) const;

  virtual bool check() const override;
};

struct completed : public StackObj {
  template <typename FSM, typename State>
  auto operator()(FSM const& fsm, State const&) const
      -> decltype(fsm.is_tending_completed(), bool());

  template <typename FSM, typename State>
  auto operator()(FSM const& fsm, State const&) const
      -> decltype(fsm.enclosing_fsm().is_tending_completed(), bool());
};
}  // namespace tending
}  // namespace guard

NAMESPACE_END

#endif  // APP_GUARD_HPP_
