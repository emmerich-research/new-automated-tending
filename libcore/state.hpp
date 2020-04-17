#ifndef LIB_CORE_STATE_HPP_
#define LIB_CORE_STATE_HPP_

/** \file state.hpp
 *  \brief State singleton class definition
 *
 * Hold all machine's state
 */

#include <iostream>

#include "common.hpp"

#include "allocation.hpp"
#include "macros.hpp"

NAMESPACE_BEGIN

namespace impl {
/**
 * \brief State implementation.
 *        This is a class wrapper that should not be instantiated and accessed
 * publicly.
 *
 * Machine's state that contains all the information regarding the machine
 *
 * @author Ray Andrew
 * @date   April 2020
 */
class StateImpl {
 public:
  /**
   * StateImpl Constructor
   *
   * @todo implemented soon
   */
  explicit StateImpl();

  /**
   * Print Function
   *
   * @todo Deleted soon
   */
  void print();
};
}  // namespace impl

/**
 * @brief Singleton of impl::StateImpl class
 *
 * Public and singleton class of impl::StateImpl class
 *
 * @author Ray Andrew
 * @date   April 2020
 */
class State : public StaticObj {
 public:
  /**
   * State singleton initialization
   *
   * @param  args    arguments are same with impl::StateImpl()
   * @return impl::StateImpl pointer
   */
  template <typename... Args>
  static impl::StateImpl* create(Args... args) {
    massert(instance_ == nullptr, "create only can be called once");
    if (instance_ == nullptr) {
      static impl::StateImpl state(args...);
      instance_ = &state;
    }
    massert(instance_ != nullptr, "sanity check");
    return instance_;
  }

  /**
   * Get impl::StateImpl pointer
   *
   * @return impl::StateImpl pointer that has been initialized
   */
  static impl::StateImpl* get() {
    massert(instance_ != nullptr, "can only be called if it is initialized");
    return instance_;
  }

 private:
  static impl::StateImpl* instance_;
};

NAMESPACE_END

#endif
