#ifndef LIB_GUI_PLC_TRIGGER_WINDOW_HPP_
#define LIB_GUI_PLC_TRIGGER_WINDOW_HPP_

#include <libcore/core.hpp>

#include <libmachine/machine.hpp>

#include "window.hpp"

NAMESPACE_BEGIN

namespace gui {
// forward declarations
class Manager;

class PLCTriggerWindow : public Window {
 public:
  /**
   * PLC Trigger Window constructor
   *
   * @param width  window width
   * @param height window height
   * @param flags  window flags
   */
  PLCTriggerWindow(float                   width = 500,
                   float                   height = 100,
                   const ImGuiWindowFlags& flags = 0);
  /**
   * PLC Trigger Window destructor
   */
  virtual ~PLCTriggerWindow() override;
  /**
   * Show contents
   *
   * @param manager ui manager
   */
  virtual void show(Manager* manager) override;
};
}  // namespace gui

NAMESPACE_END

#endif  // LIB_GUI_PLC_TRIGGER_WINDOW_HPP_
