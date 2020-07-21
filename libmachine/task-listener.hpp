#ifndef LIB_MACHINE_TASK_LISTENER_HPP_
#define LIB_MACHINE_TASK_LISTENER_HPP_

#include <thread>

#include <libcore/core.hpp>

#include "state.hpp"

NAMESPACE_BEGIN

namespace machine {
class TaskListener : public Listener {
 public:
  /**
   * Task listener constructor
   *
   * @param tsm tending state machine
   */
  TaskListener(tending* tsm);
  /**
   * Task listener destructor
   */
  virtual ~TaskListener() override;
  /**
   * Start listener
   */
  virtual void start() override;
  /**
   * Stop listener
   */
  virtual void stop() override;

 private:
  /**
   * Get state machine
   *
   * @return state machine
   */
  inline tending* tsm() const { return tsm_; }
  /**
   * Get mutex (const)
   *
   * @return mutex (const)
   */
  inline const std::mutex& mutex() const { return mutex_; }
  /**
   * Get mutex
   *
   * @return mutex
   */
  inline std::mutex& mutex() { return mutex_; }
  /**
   * Get signal (const)
   *
   * @return signal (const)
   */
  inline const std::condition_variable& signal() const { return signal_; }
  /**
   * Get signal
   *
   * @return signal
   */
  inline std::condition_variable& signal() { return signal_; }
  /**
   * Execute listener tasks
   */
  void execute();

 private:
  /**
   * Tending state machine
   */
  tending* tsm_;
  /**
   * Mutex
   */
  mutable std::mutex mutex_;
  /**
   * Signal
   */
  std::condition_variable signal_;
};
}  // namespace machine

NAMESPACE_END

// case 2: homing
//         can be longer than expected
//         if motor is stopped or hardware related issues
// if (state->homing()) {
//   LOG_ERROR("[FAULT]")
// }

#endif  // LIB_MACHINE_TASK_LISTENER_HPP_
