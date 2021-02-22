#ifndef LIB_GUI_METADATA_WINDOW_HPP_
#define LIB_GUI_METADATA_WINDOW_HPP_

#include <libcore/core.hpp>

#include "window.hpp"

NAMESPACE_BEGIN

namespace gui {
// forward declarations
class Manager;

class MetadataWindow : public Window {
 public:
  /**
   * Metadata Window constructor
   *
   * @param width  window width
   * @param height window height
   * @param flags  window flags
   */
  MetadataWindow(float                   width = 500,
                 float                   height = 100,
                 const ImGuiWindowFlags& flags = 0);
  /**
   * Metadata Window destructor
   */
  virtual ~MetadataWindow() override;
  /**
   * Show contents
   *
   * @param manager ui manager
   */
  virtual void show(Manager* manager) override;
};
}  // namespace gui

NAMESPACE_END

#endif  // LIB_GUI_METADATA_WINDOW_HPP_
