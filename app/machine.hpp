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

  /**
   * @brief Spaying Machine State implementation.
   *
   * Machine state of spraying action
   *
   * @author Ray Andrew
   * @date   June 2020
   */
  struct spraying : state_machine<spraying> {
    struct idle : state<idle> {
      template <typename FSM>
      void on_enter(event::spraying::start&& event, FSM& fsm) const;
    };
    struct preparation : state<preparation> {
      template <typename FSM>
      void on_enter(event::spraying::prepare&& event, FSM& fsm) const;

      template <typename FSM>
      void on_exit(event::spraying::run&& event, FSM& fsm) const;
    };
    struct ongoing : state<ongoing> {};

    using initial_state = idle;
    using transitions = transition_table<
        /* State, Event, Next, Action, Guard */
        tr<idle, event::spraying::prepare, preparation, guard::spraying_height>,
        tr<preparation, event::spraying::run, ongoing>,
        tr<ongoing, event::spraying::finish, idle>>;
  };

  /**
   * @brief Tending Machine State implementation.
   *
   * Machine state of tending action
   *
   * @author Ray Andrew
   * @date   June 2020
   */
  // struct tending : state_machine<tending> {
  //   struct idle : state<idle> {};
  //   struct preparation : state<preparation> {
  //     template <typename FSM>
  //     void on_enter(event::tending::prepare&& prepare, FSM& fsm) const;

  //     template <typename FSM>
  //     void on_exit(event::tending::prepare&& prepare, FSM& fsm) const;
  //   };
  //   struct ongoing : state<ongoing> {};

  //   using initial_state = idle;
  //   using transitions = transition_table<
  //       /* State, Event, Next, Action, Guard */
  //       tr<idle, event::tending::prepare, preparation,
  //       guard::tending_height>, tr<preparation, event::tending::finish,
  //       idle>>;
  // };

  using initial_state = initial;
  using transitions = transition_table<
      /* State, Event, Next, Action, Guard */
      tr<initial, event::start, running, action::do_start>,
      tr<running, event::spraying::start, spraying, none>,
      // tr<running, event::tending::start, tending, none>,
      tr<running, event::stop, terminated, action::do_stop>>;

  // caller
  tending_fsm&       rebind();
  tending_fsm const& rebind() const;

  void start();
  void stop();

  // spraying
  void start_spraying();
  void finish_spraying();

  // tending
  void start_tending();
  void finish_tending();

  // checking
  bool is_running();
  bool is_terminated();

  static const int VERSION;
};

using tending = afsm::state_machine<TendingDef>;
}  // namespace machine

NAMESPACE_END

#endif  // APP_MACHINE_HPP_
