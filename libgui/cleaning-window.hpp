#ifndef LIB_GUI_CLEANING_WINDOW_HPP_
#define LIB_GUI_CLEANING_WINDOW_HPP_

#include <libcore/core.hpp>

#include "window.hpp"

NAMESPACE_BEGIN

namespace gui {
// forward declarations
class Manager;

class CleaningWindow : public Window {
 public:
  /**
   * Cleaning Window constructor
   *
   * @param width  window width
   * @param height window height
   * @param flags  window flags
   */
  CleaningWindow(float                   width = 500,
                 float                   height = 100,
                 const ImGuiWindowFlags& flags = 0);
  /**
   * Cleaning Window destructor
   */
  virtual ~CleaningWindow() override;
  /**
   * Show contents
   *
   * @param manager ui manager
   */
  virtual void show(Manager* manager) override;
};
}  // namespace gui

NAMESPACE_END

#endif  // LIB_GUI_CLEANING_WINDOW_HPP_
