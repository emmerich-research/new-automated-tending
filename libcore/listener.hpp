#ifndef LIB_CORE_LISTENER_HPP_
#define LIB_CORE_LISTENER_HPP_

#include <atomic>
#include <thread>

#include "allocation.hpp"

NAMESPACE_BEGIN

/**
 * @brief Listener abstract class
 *
 * Definition of Listener
 *
 * @author Ray Andrew
 * @date   July 2020
 */
class Listener : public StackObj {
 public:
  /**
   * Listener constructor
   */
  Listener();
  /**
   * Listener destructor
   */
  virtual ~Listener();
  /**
   * Start listener
   */
  virtual void start() = 0;
  /**
   * Stop listener
   */
  virtual void stop() = 0;

 protected:
  /**
   * Get thread (const)
   *
   * @return thread (const)
   */
  inline const std::thread& thread() const { return thread_; }
  /**
   * Get thread
   *
   * @return thread
   */
  inline std::thread& thread() { return thread_; }
  /**
   * Get running status
   *
   * @return running
   */
  inline const std::atomic<bool>& running() const { return running_; }

 protected:
  /**
   * Running
   */
  std::atomic<bool> running_;
  /**
   * Thread
   */
  std::thread thread_;
};

NAMESPACE_END

#endif  // LIB_CORE_LISTENER_HPP_
