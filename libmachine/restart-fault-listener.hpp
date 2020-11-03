#ifndef LIB_MACHINE_RESTART_FAULT_LISTENER_HPP_
#define LIB_MACHINE_RESTART_FAULT_LISTENER_HPP_

#include <libcore/core.hpp>

#include "state.hpp"

NAMESPACE_BEGIN

namespace machine {
class RestartFaultListener : public Listener {
 public:
  /**
   * Restart fault listener constructor
   *
   * @param tsm tending state machine
   */
  RestartFaultListener(tending* tsm);
  /**
   * Restart fault listener destructor
   */
  virtual ~RestartFaultListener() override;
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
   * Get mutex
   *
   * @return state machine
   */
  inline std::mutex& mutex() { return mutex_; }
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
  std::mutex mutex_;
};
}  // namespace machine

NAMESPACE_END

#endif  // LIB_MACHINE_RESTART_FAULT_LISTENER_HPP_
