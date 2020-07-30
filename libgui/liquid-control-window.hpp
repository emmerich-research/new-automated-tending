#ifndef LIB_GUI_LIQUID_CONTROL_WINDOW_HPP_
#define LIB_GUI_LIQUID_CONTROL_WINDOW_HPP_

#include <libcore/core.hpp>
#include <libmachine/machine.hpp>

#include "window.hpp"

NAMESPACE_BEGIN

namespace gui {
// forward declarations
class Manager;

class LiquidControlWindow : public Window {
 public:
  /**
   * Liquid Control Window constructor
   *
   * @param tsm    state machine
   * @param width  window width
   * @param height window height
   * @param flags  window flags
   */
  LiquidControlWindow(machine::tending*       tsm,
                      float                   width = 500,
                      float                   height = 100,
                      const ImGuiWindowFlags& flags = 0);
  /**
   * Liquid Control Window destructor
   */
  virtual ~LiquidControlWindow() override;
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

 private:
  /**
   * State machine
   */
  machine::tending* tsm_;
};
}  // namespace gui

NAMESPACE_END

#endif  // LIB_GUI_LIQUID_CONTROL_WINDOW_HPP_
