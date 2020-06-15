#ifndef APP_MACHINE_HPP_
#define APP_MACHINE_HPP_

#include <iostream>
#include <stdexcept>
#include <thread>

#include <afsm/fsm.hpp>

#include <libcore/core.hpp>

#include "action.hpp"
#include "event.hpp"
#include "guard.hpp"

NAMESPACE_BEGIN
// struct StateMachine : public StackObj {
//   virtual ~common_base() = default;
//   virtual void do_task() = 0;
//   virtual void do_b(int) = 0;
// };

void shutdown_hook();

namespace machine {
struct TendingDef : public StackObj,
                    afsm::def::state_machine<TendingDef, std::mutex> {
  using tending_fsm = afsm::state_machine<TendingDef>;

  struct initial : state<initial> {};
  struct running : state<running> {};
  struct terminated : terminal_state<terminated> {};
  // struct tending : afsm::state_machine<tending> {};

  using initial_state = initial;
  using transitions = transition_table<
      /* State, Event, Next, Action */
      tr<initial, event::start, running, action::do_start>,
      tr<running, event::stop, terminated, action::do_stop>>;

  // caller
  tending_fsm&       rebind() { return static_cast<tending_fsm&>(*this); }
  tending_fsm const& rebind() const {
    return static_cast<tending_fsm const&>(*this);
  }

  void start();
  void stop();

  bool is_running();
  bool is_terminated();

  static const int VERSION;
};

using tending = afsm::state_machine<TendingDef>;
}  // namespace machine

NAMESPACE_END

#endif  // APP_MACHINE_HPP_
