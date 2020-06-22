#include "gui.hpp"

#include "window.hpp"

#include "util.hpp"
#include "widget.hpp"

NAMESPACE_BEGIN

namespace gui {
void control_panel_window() {
  reset_window_pos();

  auto [width, height] = get_window_size();

  ImGuiWindowFlags window_flags = 0;
  window_flags |= ImGuiWindowFlags_NoTitleBar;
  window_flags |= ImGuiWindowFlags_NoScrollbar;
  window_flags |= ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_NoResize;
  window_flags |= ImGuiWindowFlags_NoCollapse;
  window_flags |= ImGuiWindowFlags_NoNav;
  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
  window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
  // if (no_close)           p_open = NULL; // Don't pass our bool* to Begin

  ImGui::SetNextWindowSize(
      ImVec2{static_cast<float>(width), static_cast<float>(height)});
  // ImGui::SetNextWindowContentSize(ImVec2{window_width, window_height});

  if (!ImGui::Begin("Control Panel", NULL, window_flags)) {
    ImGui::End();
    return;
  }

  ImGui::Text("CNC Status");
  ImGui::SameLine();
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() -
                       ImGui::CalcTextSize("Manual Movement").x -
                       ImGui::GetScrollX() -
                       2 * ImGui::GetStyle().ItemSpacing.x);
  ImGui::Text("Manual Movement");

  // ImGui::SetNextWindowContentSize(ImVec2(width, 0.0f));
  ImGui::Columns(1, "widget_box", /* v_borders */ true);
  ImGui::Columns(2, "widget", /* v_borders */ true);
  // ImGui::Separator();
  widget::status();
  ImGui::NextColumn();
  widget::manual();
  // ImGui::NextColumn();

  {
    // Bottom Row
    ImGui::Columns(1);
    ImGui::Separator();

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }

  ImGui::End();
}
}  // namespace gui

NAMESPACE_END
