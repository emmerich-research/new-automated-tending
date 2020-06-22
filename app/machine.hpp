#ifndef APP_MACHINE_HPP_
#define APP_MACHINE_HPP_

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
// struct StateMachine : public StackObj {
//   virtual ~common_base() = default;
//   virtual void do_task() = 0;
//   virtual void do_b(int) = 0;
// };

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

  struct stopped : state<stopped> {};

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

      spraying();

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
      struct ongoing : state<ongoing> {
        // template <typename FSM>
        // void on_enter(event::spraying::run const&& event, FSM& fsm) const;

        // template <typename Event, typename FSM>
        // void on_exit(Event const&& event, FSM const& fsm) const;
      };
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

      bool initialized;
      void initialize();

      std::shared_ptr<device::DigitalOutputDevice> spraying_ready;
      std::shared_ptr<device::DigitalOutputDevice> spraying_running;
      std::shared_ptr<device::DigitalOutputDevice> spraying_complete;
      std::shared_ptr<device::DigitalOutputDevice> spray;
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

      tending();

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

      bool initialized;
      void initialize();

      std::shared_ptr<device::DigitalOutputDevice> tending_ready;
      std::shared_ptr<device::DigitalOutputDevice> tending_running;
      std::shared_ptr<device::DigitalOutputDevice> tending_complete;
      std::shared_ptr<device::PWMDevice>           finger;
    };

    using initial_state = no_task;
    using transitions =
        transition_table<tr<no_task, event::spraying::start, spraying>,
                         tr<spraying, event::task_complete, no_task>,
                         tr<tending, event::task_complete, no_task>,
                         tr<no_task, event::tending::start, tending>>;

    /**
     * Constructor
     */
    running();
    /**
     * Check if spraying is completed
     *
     * @return spraying is completed or not
     */
    bool is_spraying_completed() const;
    /**
     * Check if tending completed
     *
     * @return tending completed or not
     */
    bool is_tending_completed() const;
    /**
     * This will become true if only spraying has
     * been completed
     */
    bool is_spraying_completed_;
    /**
     * This will become true if only tending has
     * been completed
     */
    bool is_tending_completed_;
  };

  using initial_state = initial;
  using transitions = transition_table<
      /* State, Event, Next, Action, Guard */
      tr<initial, event::start, running, action::start>,
      tr<running, event::fault, stopped, action::fault>,
      tr<stopped, event::restart, running, action::restart>,
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
   * Check if machine is ready
   * This denotes as "all devices are ready"
   *
   * @return machine is ready or not
   */
  bool is_ready() const;
  /**
   * Check if machine is running
   *
   * @return machine is running or not
   */
  bool is_running() const;
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

#endif  // APP_MACHINE_HPP_
