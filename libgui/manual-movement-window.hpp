#ifndef LIB_GUI_MANUAL_MOVEMENT_WINDOW_HPP_
#define LIB_GUI_MANUAL_MOVEMENT_WINDOW_HPP_

#include <libcore/core.hpp>

#include <libmachine/machine.hpp>

#include <libalgo/algo.hpp>

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
