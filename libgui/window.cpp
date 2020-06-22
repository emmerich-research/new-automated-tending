#include "gui.hpp"

#include "window.hpp"

#include "util.hpp"
#include "widget.hpp"

NAMESPACE_BEGIN

namespace gui {
void control_panel_window() {
  gui::reset_window_pos();

  auto [width, height] = gui::get_window_size();

  ImGui::SetNextWindowSize(
      ImVec2{static_cast<float>(width), static_cast<float>(height)});
  // ImGui::SetNextWindowContentSize(ImVec2{window_width, window_height});

  ImGui::Begin("Control Panel");

  widget::status();
  widget::manual();

  ImGui::End();
}
}  // namespace gui

NAMESPACE_END
