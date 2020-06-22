#ifndef LIB_CORE_STATE_HPP_
#define LIB_CORE_STATE_HPP_

/** @file state.hpp
 *  @brief State singleton class definition
 *
 * Hold all machine's state
 */

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
namespace impl {
class StateImpl;
}

using State = StaticObj<impl::StateImpl>;

using Point = double;

/**
 * @brief Coordinate
 *
 * Keeping tracks of coordinate of the machine
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

struct Task {
  bool ready;
  bool running;
  bool complete;
  bool fault;
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

  /**
   * Return the latest path movement id
   *
   * @return latest path id
   */
  unsigned int path_id();
  /**
   * Set the latest path id
   *
   * @param path_id  set the latest path id
   */
  void path_id(unsigned int path_id);
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
   * @param x set new x-axis coordinate
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
   * Set spraying fault
   *
   * @param fault fault status (true or false)
   */
  void spraying_fault(bool fault);
  /**
   * Get spraying fault
   *
   * @return status of spraying fault
   */
  bool spraying_fault();
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
   * Set tending fault
   *
   * @param fault fault status (true or false)
   */
  void tending_fault(bool fault);
  /**
   * Get tending fault
   *
   * @return status of tending fault
   */
  bool tending_fault();
  /**
   * Get fault status
   *
   * @return status of fault
   */
  bool fault();
  /**
   * Set manual mode
   */
  void manual_mode(bool manual);
  /**
   * Manual mode
   *
   * @return status of manual mode
   */
  bool manual_mode();

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
   * Get mutex
   *
   * @return state mutex
   */
  StateMutex& mutex();

 private:
  /**
   * Latest path id
   */
  unsigned int path_id_;
  /**
   * Current coordinate
   */
  Coordinate coordinate_;
  /**
   * State read mutex
   */
  StateMutex mutex_;
  /**
   * Tending task
   */
  Task tending_;
  /**
   * Spraying task
   */
  Task spraying_;
  /**
   * Manual mode
   */
  bool manual_mode_;
};
}  // namespace impl

/** impl::StateImpl singleton class using StaticObj */

NAMESPACE_END

#endif
