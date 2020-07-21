#ifndef LIB_MACHINE_FAULT_LISTENER_HPP_
#define LIB_MACHINE_FAULT_LISTENER_HPP_

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
   * Execute listener tasks
   */
  void execute();

 private:
  /**
   * Tending state machine
   */
  tending* tsm_;
};
}  // namespace machine

NAMESPACE_END

#endif  // LIB_MACHINE_FAULT_LISTENER_HPP_
