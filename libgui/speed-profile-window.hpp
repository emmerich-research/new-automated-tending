#ifndef LIB_GUI_SPEED_PROFILE_WINDOW_HPP_
#define LIB_GUI_SPEED_PROFILE_WINDOW_HPP_

#include <libcore/core.hpp>
#include <libmachine/machine.hpp>

#include "window.hpp"

NAMESPACE_BEGIN

namespace gui {
// forward declarations
class Manager;

class SpeedProfileWindow : public Window {
 public:
  /**
   * Speed Profile Window constructor
   *
   * @param tsm    state machine
   * @param width  window width
   * @param height window height
   * @param flags  window flags
   */
  SpeedProfileWindow(const machine::tending* tsm,
                     float                   width = 500,
                     float                   height = 100,
                     const ImGuiWindowFlags& flags = 0);
  /**
   * Speed Profile Window destructor
   */
  virtual ~SpeedProfileWindow() override;
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
  inline const machine::tending* tsm() const { return tsm_; }

 private:
  /**
   * State machine
   */
  const machine::tending* tsm_;
};
}  // namespace gui

NAMESPACE_END

#endif  // LIB_GUI_SPEED_PROFILE_WINDOW_HPP_
