#ifndef LIB_MACHINE_FAULT_LISTENER_HPP_
#define LIB_MACHINE_FAULT_LISTENER_HPP_

#include <condition_variable>
#include <thread>

#include <libcore/core.hpp>

#include "state.hpp"

NAMESPACE_BEGIN

namespace machine {
class FaultListener : public Listener {
 public:
  /**
   * Fault listener constructor
   *
   * @param tsm tending state machine
   */
  FaultListener(tending* tsm);
  /**
   * Fault listener destructor
   */
  virtual ~FaultListener() override;
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

#endif  // LIB_MACHINE_FAULT_LISTENER_HPP_
