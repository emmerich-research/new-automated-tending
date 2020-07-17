#ifndef LIB_GUI_FAULT_WINDOW_HPP_
#define LIB_GUI_FAULT_WINDOW_HPP_

#include <libmachine/machine.hpp>

#include "window.hpp"

NAMESPACE_BEGIN

namespace gui {
// forward declarations
class Manager;

class FaultWindow : public Window {
 public:
  /**
   * Fault Window constructor
   *
   * @param tsm    state machine
   * @param width  window width
   * @param height window height
   * @param flags  window flags
   */
  FaultWindow(machine::tending*       tsm,
              float                   width = 500,
              float                   height = 100,
              const ImGuiWindowFlags& flags = 0);
  /**
   * Fault Window destructor
   */
  virtual ~FaultWindow() override;
  /**
   * Show contents
   *
   * @param manager ui manager
   */
  virtual void show(const Manager* manager) override;

 private:
  inline machine::tending* tsm() { return tsm_; }

 private:
  machine::tending* tsm_;
};
}  // namespace gui

NAMESPACE_END

#endif  // LIB_GUI_FAULT_WINDOW_HPP_
