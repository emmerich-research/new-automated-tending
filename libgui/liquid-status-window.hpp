#ifndef LIB_GUI_LIQUID_WINDOW_HPP_
#define LIB_GUI_LIQUID_WINDOW_HPP_

#include <libcore/core.hpp>

#include "window.hpp"

NAMESPACE_BEGIN

namespace gui {
// forward declarations
class Manager;

class LiquidStatusWindow : public Window {
 public:
  /**
   * Liquid Status Window constructor
   *
   * @param width  window width
   * @param height window height
   * @param flags  window flags
   */
  LiquidStatusWindow(float                   width = 500,
                     float                   height = 100,
                     const ImGuiWindowFlags& flags = 0);
  /**
   * Liquid Status Window destructor
   */
  virtual ~LiquidStatusWindow() override;
  /**
   * Show contents
   *
   * @param manager ui manager
   */
  virtual void show(Manager* manager) override;
};
}  // namespace gui

NAMESPACE_END

#endif  // LIB_GUI_LIQUID_WINDOW_HPP_
