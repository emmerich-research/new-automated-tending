#ifndef LIB_CORE_STATE_HPP_
#define LIB_CORE_STATE_HPP_

/** @file state.hpp
 *  @brief State singleton class definition
 *
 * Hold all machine's state
 */

#include <utility>

#include <libutil/util.hpp>

#include "common.hpp"

#include "allocation.hpp"

NAMESPACE_BEGIN

// forward declaration
namespace impl {
class StateImpl;
}

using State = StaticObj<impl::StateImpl>;

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
  /**
   * Return the latest path movement id
   *
   * @return latest path id
   */
  unsigned int path_id() const;
  /**
   * Set the latest path id
   *
   * @param path_id  set the latest path id
   */
  void path_id(unsigned int path_id);

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
   * Latest path id
   */
  unsigned int path_id_;
};
}  // namespace impl

/** impl::StateImpl singleton class using StaticObj */

NAMESPACE_END

#endif
