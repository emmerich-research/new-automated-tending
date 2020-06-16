#ifndef APP_GUARD_HPP_
#define APP_GUARD_HPP_

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace guard {
struct guard_base : public StackObj {
  virtual bool check() const = 0;
};

struct homing : public guard_base {
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
}  // namespace spraying

namespace tending {
struct height : public guard_base {
  template <typename FSM, typename State>
  bool operator()(FSM const& fsm, State const&) const;

  virtual bool check() const override;
};
}  // namespace tending
}  // namespace guard

NAMESPACE_END

#endif  // APP_GUARD_HPP_
