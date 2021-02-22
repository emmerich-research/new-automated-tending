#ifndef LIB_CORE_STATE_HPP_
#define LIB_CORE_STATE_HPP_

/** @file state.hpp
 *  @brief State singleton class definition
 *
 * Hold all machine's state
 */

#include <chrono>
#include <condition_variable>
#include <shared_mutex>
#include <thread>
#include <utility>

#include <libutil/util.hpp>

#include "common.hpp"

#include "allocation.hpp"

NAMESPACE_BEGIN

// forward declaration
struct Coordinate;
struct TaskState;
namespace config {
enum class speed;
}
namespace impl {
class StateImpl;
}

/** impl::StateImpl singleton class using StaticObj */
using State = StaticObj<impl::StateImpl>;

using Point = double;

/**
 * @brief Coordinate
 *
 * Keeping track of coordinate of the machine
 *
 * @author Ray Andrew
 * @date   June 2020
 */
struct Coordinate {
  /**
   * X-axis
   */
  Point x;
  /**
   * Y-axis
   */
  Point y;
  /**
   * Z-axis
   */
  Point z;
};

/**
 * @brief Task
 *
 * Keeping track of task status of machine
 *
 * @author Ray Andrew
 * @date   July 2020
 */
struct Task {
  /**
   * Ready
   */
  bool ready;
  /**
   * Running
   */
  bool running;
  /**
   * Complete
   */
  bool complete;
  /**
   * Task Constructor
   */
  Task();
  /**
   * Reset Task status
   */
  void reset();
};

/**
 * @brief Refill Task
 *
 * Refill task status
 *
 * @author Ray Andrew
 * @date   July 2020
 */
struct Refill {
  /**
   * Schedule type
   */
  typedef enum { ONE_DAY = 1, TWO_DAYS, THREE_DAYS, OFF = 3650 } Schedule;
  /**
   * Requested
   */
  bool requested;
  /**
   * Running
   */
  bool running;
  /**
   * Schedule
   */
  Schedule schedule;
  /**
   * Last refill
   */
  TimePoint last;
  /**
   * Next refill
   */
  TimePoint next;
  /**
   * Refill constructor
   */
  Refill();
  /**
   * Change schedule
   *
   * Will recalculate next refill time
   *
   * @param new_schedule new schedule
   */
  void set_schedule(const Schedule& new_schedule);
  /**
   * Change schedule
   *
   * Will recalculate next refill time
   *
   * @param time new time
   */
  void set_last(const TimePoint& time = Clock::now());
  /**
   * Update time
   *
   * Recalculate next refill time
   */
  void update();
  /**
   * Reset Task status
   */
  void reset();
};

namespace impl {
/**
 * @brief State implementation.
 *        This is a class wrapper that should not be instantiated and accessed
 * publicly.
 *
 * Global state of machine (not machine state)
 *
 * @author Ray Andrew
 * @date   April 2020
 */
class StateImpl : public StackObj {
  template <class StateImpl>
  template <typename... Args>
  friend ATM_STATUS StaticObj<StateImpl>::create(Args&&... args);

 public:
  using StateMutex = std::shared_mutex;
  using StateLock = std::lock_guard<StateMutex>;
  using Signal = std::condition_variable;

  /**
   * Reset ui state
   */
  void reset_ui();
  /**
   * Get signal
   *
   * @return signal
   */
  Signal& signal();
  /**
   * Notify one thread
   */
  void notify_one();
  /**
   * Notify all threads
   */
  void notify_all();
  /**
   * Get mutex
   *
   * @return state mutex
   */
  StateMutex& mutex();

  /**
   * Return running status
   *
   * @return running status
   */
  bool running();
  /**
   * Set the running status
   *
   * @param run  set the running status
   */
  void running(bool run);
  /**
   * Set new coordinate
   *
   * @param coordinate set new coordinate
   */
  void coordinate(const Coordinate& coordinate);
  /**
   * Get coordinate
   *
   * @return current coordinate
   */
  const Coordinate& coordinate();
  /**
   * Reset coordinate
   */
  void reset_coordinate();
  /**
   * Set x-axis coordinate
   *
   * @param x set new x-axis coordinate
   */
  void x(const Point& x);
  /**
   * Increment x-axis coordinate
   */
  void inc_x();
  /**
   * Decrement x-axis coordinate
   */
  void dec_x();
  /**
   * Get x-axis coordinate
   *
   * @return  x-axis coordinate
   */
  const Point& x();
  /**
   * Set y-axis coordinate
   *
   * @param y set new y-axis coordinate
   */
  void y(const Point& y);
  /**
   * Increment y-axis coordinate
   */
  void inc_y();
  /**
   * Decrement y-axis coordinate
   */
  void dec_y();
  /**
   * Get y-axis coordinate
   *
   * @return  y-axis coordinate
   */
  const Point& y();
  /**
   * Set z-axis coordinate
   *
   * @param z set new x-axis coordinate
   */
  void z(const Point& z);
  /**
   * Increment z-axis coordinate
   */
  void inc_z();
  /**
   * Decrement z-axis coordinate
   */
  void dec_z();
  /**
   * Get x-axis coordinate
   *
   * @return  x-axis coordinate
   */
  const Point& z();
  /**
   * Get spraying task
   *
   * @return spraying task
   */
  const Task& spraying();
  /**
   * Set spraying ready
   *
   * @param ready ready status (true or false)
   */
  void spraying_ready(bool ready);
  /**
   * Get spraying ready
   *
   * @return status of spraying ready
   */
  bool spraying_ready();
  /**
   * Set spraying running
   *
   * @param running running status (true or false)
   */
  void spraying_running(bool running);
  /**
   * Get spraying running
   *
   * @return status of spraying running
   */
  bool spraying_running();
  /**
   * Set spraying complete
   *
   * @param complete complete status (true or false)
   */
  void spraying_complete(bool complete);
  /**
   * Get spraying complete
   *
   * @return status of spraying complete
   */
  bool spraying_complete();
  /**
   * Get tending task
   *
   * @return tending task
   */
  const Task& tending();
  /**
   * Set tending ready
   *
   * @param ready ready status (true or false)
   */
  void tending_ready(bool ready);
  /**
   * Get tending fault
   *
   * @return status of tending fault
   */
  bool tending_ready();
  /**
   * Set tending running
   *
   * @param running running status (true or false)
   */
  void tending_running(bool running);
  /**
   * Get tending running
   *
   * @return status of tending running
   */
  bool tending_running();
  /**
   * Set tending complete
   *
   * @param complete complete status (true or false)
   */
  void tending_complete(bool complete);
  /**
   * Get tending complete
   *
   * @return status of tending complete
   */
  bool tending_complete();
  /**
   * Get cleaning task
   *
   * @return cleaning task
   */
  const Task& cleaning();
  /**
   * Set cleaning ready
   *
   * @param ready ready status (true or false)
   */
  void cleaning_ready(bool ready);
  /**
   * Get cleaning fault
   *
   * @return status of cleaning fault
   */
  bool cleaning_ready();
  /**
   * Set cleaning running
   *
   * @param running running status (true or false)
   */
  void cleaning_running(bool running);
  /**
   * Get cleaning running
   *
   * @return status of cleaning running
   */
  bool cleaning_running();
  /**
   * Set cleaning complete
   *
   * @param complete complete status (true or false)
   */
  void cleaning_complete(bool complete);
  /**
   * Get cleaning complete
   *
   * @return status of cleaning complete
   */
  bool cleaning_complete();
  /**
   * Set fault status
   *
   * @param fault fault status (true or false)
   */
  void fault(bool fault);
  /**
   * Get fault status
   *
   * @return status of fault
   */
  bool fault();
  /**
   * Set manual mode
   *
   * @param manual manual mode status
   */
  void manual_mode(bool manual);
  /**
   * Manual mode
   *
   * @return status of manual mode
   */
  bool manual_mode();
  /**
   * Set homing status
   *
   * @param status homing status
   */
  void homing(bool status);
  /**
   * Homing status
   *
   * @return status of homing
   */
  bool homing();
  /**
   * Set profile speed
   */
  void speed_profile(const config::speed& speed_profile);
  /**
   * Get profile speed
   *
   * @return profile speed
   */
  const config::speed& speed_profile();
  /**
   * Get water refilling status
   *
   * @return water refilling status
   */
  const Refill& water_refilling();
  /**
   * Set water refilling request status
   *
   * @param request refilling request status (true or false)
   */
  void water_refilling_request(bool request);
  /**
   * Get water refilling request
   *
   * @return status of water refilling request
   */
  bool water_refilling_requested();
  /**
   * Set water refilling running status
   *
   * @param refilling refilling running status (true or false)
   */
  void water_refilling_running(bool refilling);
  /**
   * Get water refilling running
   *
   * @return status of water refilling running
   */
  bool water_refilling_running();
  /**
   * Get water refilling schedule
   *
   * @return schedule of water refilling
   */
  const Refill::Schedule& water_refilling_schedule();
  /**
   * Set water refilling schedule
   *
   * @param schedule schedule of water refilling
   */
  void water_refilling_schedule(const Refill::Schedule& schedule);
  /**
   * Set water refilling last executed time
   *
   * @param time last executed time of water refilling
   */
  void water_refilling_last_executed(const TimePoint& time = Clock::now());
  /**
   * Get water refilling next executed time
   *
   * @return next executed time of water refilling
   */
  const TimePoint& water_refilling_next_executed();
  /**
   * Get disinfectant refilling status
   *
   * @return disinfectant refilling status
   */
  const Refill& disinfectant_refilling();
  /**
   * Set disinfectant refilling request status
   *
   * @param request refilling request status (true or false)
   */
  void disinfectant_refilling_request(bool request);
  /**
   * Get disinfectant refilling request
   *
   * @return status of disinfectant refilling request
   */
  bool disinfectant_refilling_requested();
  /**
   * Set disinfectant refilling running status
   *
   * @param refilling refilling running status (true or false)
   */
  void disinfectant_refilling_running(bool refilling);
  /**
   * Get disinfectant refilling running
   *
   * @return status of disinfectant refilling running
   */
  bool disinfectant_refilling_running();
  /**
   * Get disinfectant refilling schedule
   *
   * @return schedule of disinfectant refilling
   */
  const Refill::Schedule& disinfectant_refilling_schedule();
  /**
   * Set disinfectant refilling schedule
   *
   * @param schedule schedule of disinfectant refilling
   */
  void disinfectant_refilling_schedule(const Refill::Schedule& schedule);
  /**
   * Set disinfectant refilling last executed time
   *
   * @param time last executed time of disinfectant refilling
   */
  void disinfectant_refilling_last_executed(
      const TimePoint& time = Clock::now());
  /**
   * Get disinfectant refilling last executed time
   *
   * @return last executed time of disinfectant refilling
   */
  const TimePoint& disinfectant_refilling_next_executed();

 private:
  /**
   * StateImpl Constructor
   *
   * @todo implemented soon
   */
  explicit StateImpl();
  /**
   * StateImpl Destructor
   *
   * Noop
   *
   */
  ~StateImpl() = default;

 private:
  /**
   * Speed profile
   */
  config::speed speed_profile_;
  /**
   * Running
   */
  bool running_;
  /**
   * Current coordinate
   */
  Coordinate coordinate_;
  /**
   * State read mutex
   */
  StateMutex mutex_;
  /**
   * Signal
   */
  Signal signal_;
  /**
   * Tending task
   */
  Task tending_;
  /**
   * Spraying task
   */
  Task spraying_;
  /**
   * Cleaning task
   */
  Task cleaning_;
  /**
   * Fault status
   */
  bool fault_;
  /**
   * Manual mode
   */
  bool manual_mode_;
  /**
   * Homing
   */
  bool homing_;
  /**
   * Water refilling
   */
  Refill water_refilling_;
  /**
   * Disinfectant refilling
   */
  Refill disinfectant_refilling_;
};
}  // namespace impl

NAMESPACE_END

#endif
