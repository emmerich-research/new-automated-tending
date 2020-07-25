#ifndef LIB_GUI_MANUAL_MOVEMENT_WINDOW_HPP_
#define LIB_GUI_MANUAL_MOVEMENT_WINDOW_HPP_

#include <libalgo/algo.hpp>
#include <libcore/core.hpp>
#include <libmachine/machine.hpp>

#include "window.hpp"

NAMESPACE_BEGIN

namespace gui {
// forward declarations
class Manager;

class ManualMovementWindow : public Window {
 public:
  /**
   * Manual Movement Window constructor
   *
   * @param tsm    state machine
   * @param width  window width
   * @param height window height
   * @param flags  window flags
   */
  ManualMovementWindow(machine::tending*       tsm,
                       float                   width = 500,
                       float                   height = 100,
                       const ImGuiWindowFlags& flags = 0);
  /**
   * Manual Movement Window destructor
   */
  virtual ~ManualMovementWindow() override;
  /**
   * Show contents
   *
   * @param manager ui manager
   */
  virtual void show(Manager* manager) override;

 private:
  /**
   * Get state machine
   *
   * @return state machine
   */
  inline machine::tending* tsm() { return tsm_; }
  /**
   * Get thread pool
   *
   * @return thread pool
   */
  inline algo::ThreadPool& thread_pool() { return thread_pool_; }
  /**
   * Move single or multi steppers at once
   *
   * @tparam Unit movement unit (cm / mm)
   *
   * @param x  length of x-axis
   * @param y  length of y-axis
   * @param z  length of z-axis
   */
  template <mechanism::movement::unit Unit>
  inline void move(Point x, Point y, Point z) const {
    massert(State::get() != nullptr, "sanity");
    massert(Config::get() != nullptr, "sanity");

    auto*       state = State::get();
    const auto* config = Config::get();
    auto&&      movement = mechanism::movement_mechanism();

    movement->motor_profile(
        config->fault_speed_profile(state->speed_profile()));
    movement->move<Unit>(x, y, z);
  }

 private:
  /**
   * State machine
   */
  machine::tending* tsm_;
  /**
   * Movement thread pool
   */
  algo::ThreadPool thread_pool_;
};
}  // namespace gui

NAMESPACE_END

#endif  // LIB_GUI_MANUAL_MOVEMENT_WINDOW_HPP_
