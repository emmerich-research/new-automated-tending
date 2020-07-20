#ifndef LIB_MACHINE_STATE_HPP_
#define LIB_MACHINE_STATE_HPP_

#include <iostream>
#include <stdexcept>
#include <thread>

#include <afsm/fsm.hpp>

#include <libalgo/algo.hpp>
#include <libcore/core.hpp>

#include "action.hpp"
#include "event.hpp"
#include "guard.hpp"

NAMESPACE_BEGIN

namespace machine {
/**
 * @brief Machine State Machine Definition.
 *
 * Definition of Machine State
 *
 * @author Ray Andrew
 * @date   June 2020
 */
struct TendingDef : public StackObj,
                    afsm::def::state_machine<TendingDef, std::mutex> {
  /**
   * A type alias for actual state machine, that will be passed
   * to actions and guards
   **/
  using tending_fsm = afsm::state_machine<TendingDef>;

  struct initial : state<initial> {
    template <typename Event, typename FSM>
    void on_enter(Event const&& event, FSM& fsm) const;
  };

  struct fault : state_machine<fault> {
    /**
     * A type alias for actual state machine, that will be passed
     * to actions and guards
     **/
    using fault_fsm = afsm::inner_state_machine<fault, tending_fsm>;

    struct idle : state<idle> {
      template <typename Event, typename FSM>
      void on_enter(Event const&& event, FSM& fsm) const;
    };

    struct manual : state<manual> {
      template <typename Event, typename FSM>
      void on_enter(Event const&& event, FSM& fsm) const;

      template <typename Event, typename FSM>
      void on_exit(Event const&& event, FSM& fsm) const;
    };

    using initial_state = idle;
    using transitions = transition_table<
        /* State, Event, Next, Action, Guard */
        tr<idle, event::fault::manual, manual>>;
  };

  struct terminated : terminal_state<terminated> {};

  struct running : state_machine<running> {
    /**
     * A type alias for actual state machine, that will be passed
     * to actions and guards
     **/
    using running_fsm = afsm::inner_state_machine<running, tending_fsm>;

    struct no_task : state<no_task> {
      template <typename Event, typename FSM>
      void on_enter(Event const&& event, FSM& fsm) const;
    };

    /**
     * @brief Spraying Machine State implementation.
     *
     * Machine state of spraying action
     *
     * @author Ray Andrew
     * @date   June 2020
     */
    struct spraying : state_machine<spraying> {
      using spraying_fsm = afsm::inner_state_machine<spraying, running_fsm>;

      struct idle : state<idle> {
        template <typename Event, typename FSM>
        void on_enter(Event const&& event, FSM& fsm) const;
      };
      struct preparation : state<preparation> {
        template <typename Event, typename FSM>
        void on_enter(Event&& event, FSM& fsm);

        template <typename Event, typename FSM>
        void on_exit(Event&& event, FSM const& fsm) const;
      };
      struct ongoing : state<ongoing> {};
      struct completed : terminal_state<completed> {};

      using initial_state = idle;
      using transitions = transition_table<
          /* State, Event, Next, Action, Guard */
          tr<idle, none, preparation, none>,
          tr<preparation, event::spraying::run, ongoing, action::spraying::job>,
          tr<ongoing,
             none,
             completed,
             action::spraying::complete,
             guard::spraying::completed>>;
    };

    /**
     * @brief Tending Machine State implementation.
     *
     * Machine state of tending action
     *
     * @author Ray Andrew
     * @date   June 2020
     */
    struct tending : state_machine<tending> {
      using tending_fsm = afsm::inner_state_machine<tending, running_fsm>;

      struct idle : state<idle> {
        template <typename Event, typename FSM>
        void on_enter(Event const&& event, FSM& fsm) const;
      };
      struct preparation : state<preparation> {
        template <typename Event, typename FSM>
        void on_enter(Event&& event, FSM& fsm);

        template <typename Event, typename FSM>
        void on_exit(Event&& event, FSM const& fsm) const;
      };
      struct ongoing : state<ongoing> {};
      struct completed : terminal_state<completed> {};

      using initial_state = idle;
      using transitions = transition_table<
          /* State, Event, Next, Action, Guard */
          tr<idle, none, preparation, none>,
          tr<preparation, event::tending::run, ongoing, action::tending::job>,
          tr<ongoing,
             none,
             completed,
             action::tending::complete,
             guard::tending::completed>>;
    };

    /**
     * @brief Cleaning Machine State implementation.
     *
     * Machine state of cleaning action
     *
     * @author Ray Andrew
     * @date   July 2020
     */
    struct cleaning : state_machine<cleaning> {
      using cleaning_fsm = afsm::inner_state_machine<cleaning, running_fsm>;

      struct idle : state<idle> {
        template <typename Event, typename FSM>
        void on_enter(Event const&& event, FSM& fsm) const;
      };
      struct preparation : state<preparation> {
        template <typename Event, typename FSM>
        void on_enter(Event&& event, FSM& fsm);

        template <typename Event, typename FSM>
        void on_exit(Event&& event, FSM const& fsm) const;
      };
      struct ongoing : state<ongoing> {};
      struct completed : terminal_state<completed> {};

      using initial_state = idle;
      using transitions = transition_table<
          /* State, Event, Next, Action, Guard */
          tr<idle, none, preparation, none>,
          tr<preparation, event::cleaning::run, ongoing, action::cleaning::job>,
          tr<ongoing,
             none,
             completed,
             action::cleaning::complete,
             guard::cleaning::completed>>;
    };

    using initial_state = no_task;
    using transitions =
        transition_table<tr<no_task, event::spraying::start, spraying>,
                         tr<spraying, event::task_complete, no_task>,
                         tr<no_task, event::tending::start, tending>,
                         tr<tending, event::task_complete, no_task>,
                         tr<no_task, event::cleaning::start, cleaning>,
                         tr<cleaning, event::task_complete, no_task>>;
  };

  using initial_state = initial;
  using transitions = transition_table<
      /* State, Event, Next, Action, Guard */
      tr<initial, event::start, running, action::start>,
      tr<running, event::fault::trigger, fault, action::fault, guard::fault>,
      tr<fault, event::fault::restart, running, action::restart>,
      tr<fault, event::stop, terminated, action::stop>,
      tr<running, event::stop, terminated, action::stop>>;

  /**
   * Constructor
   */
  TendingDef();
  /**
   * Get instance of this state machine
   *
   * @return instance of this state machine
   */
  tending_fsm& rebind();
  /**
   * Get constant instance of this state machine
   *
   * @return constant instance of this state machine
   */
  tending_fsm const& rebind() const;
  /**
   * Start machine
   **/
  void start();
  /**
   * Stop machine
   */
  void stop();
  /**
   * Trigger fault to machine
   */
  void fault();
  /**
   * Trigger fault to machine in manual mode
   */
  void fault_manual();
  /**
   * Restart machine after fault
   */
  void restart();
  /**
   * Trigger `no task`
   */
  void task_completed();
  /**
   * Start spraying
   */
  void start_spraying();
  /**
   * Run spraying
   */
  void run_spraying();
  /**
   * Start tending
   */
  void start_tending();
  /**
   * Run tending
   */
  void run_tending();
  /**
   * Start cleaning
   */
  void start_cleaning();
  /**
   * Run cleaning
   */
  void run_cleaning();
  /**
   * Check if machine is ready
   * This denotes as "all devices are ready"
   *
   * @return machine is ready or not
   */
  bool is_ready() const;
  /**
   * Check if machine is running
   *
   * Running condition can be idle (no_task),
   * spraying, tending, or cleaning
   *
   * as long as it is not fault
   *
   * @return machine is running or not
   */
  bool is_running() const;
  /**
   * Check if machine is having no task
   *
   * @return machine is having no task or not
   */
  bool is_no_task() const;
  /**
   * Check if machine is terminated
   *
   * @return machine is terminated or not
   */
  bool is_terminated() const;
  /**
   * Get thread pool instance
   *
   * @return instance of algo::ThreadPool
   */
  inline algo::ThreadPool& thread_pool() { return thread_pool_; }
  /**
   * Version of state machine
   */
  static const int VERSION;
  /**
   * This will become true if only devices initialization
   * done successfully
   */
  bool machine_ready_;
  /**
   * Thread Pool for running the task in separate thread
   */
  algo::ThreadPool thread_pool_;
};

using tending = afsm::state_machine<TendingDef>;
}  // namespace machine

NAMESPACE_END

#endif  // LIB_MACHINE_STATE_HPP_
