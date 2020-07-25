#ifndef LIB_GUI_LIQUID_CONTROL_WINDOW_HPP_
#define LIB_GUI_LIQUID_CONTROL_WINDOW_HPP_

#include <libcore/core.hpp>

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
   * @param width  window width
   * @param height window height
   * @param flags  window flags
   */
  LiquidControlWindow(float                   width = 500,
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
};
}  // namespace gui

NAMESPACE_END

#endif  // LIB_GUI_LIQUID_CONTROL_WINDOW_HPP_
