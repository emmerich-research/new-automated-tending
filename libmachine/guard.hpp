#ifndef LIB_MACHINE_GUARD_HPP_
#define LIB_MACHINE_GUARD_HPP_

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace guard {
struct base : public StackObj {
  virtual bool check() const = 0;
  virtual ~base() = default;
};

struct machine_ready : public StackObj {
  template <typename FSM, typename State>
  bool operator()(FSM const&, State const&) const;
};

struct e_stop : public base {
  template <typename FSM, typename State>
  bool operator()(FSM const&, State const&) const;

  virtual bool check() const override;
};

struct reset : public base {
  template <typename FSM, typename State>
  bool operator()(FSM const&, State const&) const;

  virtual bool check() const override;
};

struct fault : public base {
  template <typename FSM, typename State>
  bool operator()(FSM const&, State const&) const;

  virtual bool check() const override;
};

namespace spraying {
struct completed : public base {
  template <typename FSM, typename State>
  bool operator()(FSM const&, State const&) const;

  virtual bool check() const override;
};
}  // namespace spraying

namespace tending {
struct completed : public base {
  template <typename FSM, typename State>
  bool operator()(FSM const&, State const&) const;

  virtual bool check() const override;
};
}  // namespace tending

namespace cleaning {
struct completed : public base {
  template <typename FSM, typename State>
  bool operator()(FSM const&, State const&) const;

  virtual bool check() const override;
};
}  // namespace cleaning

namespace height {
struct spraying_tending : public base {
  template <typename FSM, typename State>
  bool operator()(FSM const&, State const&) const;

  virtual bool check() const override;
};

struct cleaning : public base {
  template <typename FSM, typename State>
  bool operator()(FSM const&, State const&) const;

  virtual bool check() const override;
};
}  // namespace height
}  // namespace guard

NAMESPACE_END

#endif  // LIB_MACHINE_GUARD_HPP_
