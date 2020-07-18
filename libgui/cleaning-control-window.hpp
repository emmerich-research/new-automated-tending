#ifndef LIB_GUI_CLEANING_CONTROL_WINDOW_HPP_
#define LIB_GUI_CLEANING_CONTROL_WINDOW_HPP_

#include <libcore/core.hpp>

#include "window.hpp"

NAMESPACE_BEGIN

namespace gui {
// forward declarations
class Manager;

class CleaningControlWindow : public Window {
 public:
  /**
   * Cleaning Control Window constructor
   *
   * @param width  window width
   * @param height window height
   * @param flags  window flags
   */
  CleaningControlWindow(float                   width = 500,
                        float                   height = 100,
                        const ImGuiWindowFlags& flags = 0);
  /**
   * Cleaning Control Window destructor
   */
  virtual ~CleaningControlWindow() override;
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

#endif  // LIB_GUI_CLEANING_CONTROL_WINDOW_HPP_
