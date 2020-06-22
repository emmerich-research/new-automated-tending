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
  bool completed;
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
};
}  // namespace impl

/** impl::StateImpl singleton class using StaticObj */

NAMESPACE_END

#endif
