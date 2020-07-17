#ifndef LIB_GUI_MOVEMENT_WINDOW_HPP_
#define LIB_GUI_MOVEMENT_WINDOW_HPP_

#include <libcore/core.hpp>

#include <libmachine/machine.hpp>

#include "window.hpp"

NAMESPACE_BEGIN

namespace gui {
// forward declarations
class Manager;

class MovementWindow : public Window {
 public:
  /**
   * Movement Window constructor
   *
   * @param tsm    state machine
   * @param width  window width
   * @param height window height
   * @param flags  window flags
   */
  MovementWindow(machine::tending*       tsm,
                 float                   width = 500,
                 float                   height = 100,
                 const ImGuiWindowFlags& flags = 0);
  /**
   * Movement Window destructor
   */
  virtual ~MovementWindow() override;
  /**
   * Show contents
   *
   * @param manager ui manager
   */
  virtual void show(const Manager* manager) override;
  /**
   * After render contents
   *
   * @param manager ui manager
   */
  virtual void after_render(const Manager* manager) override;

 private:
  inline machine::tending* tsm() { return tsm_; }

 private:
  machine::tending* tsm_;
};
}  // namespace gui

NAMESPACE_END

#endif  // LIB_GUI_MOVEMENT_WINDOW_HPP_
