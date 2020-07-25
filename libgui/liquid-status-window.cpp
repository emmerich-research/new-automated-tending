#include "gui.hpp"

#include "liquid-status-window.hpp"

#include "util.hpp"

NAMESPACE_BEGIN

namespace gui {

LiquidStatusWindow::LiquidStatusWindow(float                   width,
                                       float                   height,
                                       const ImGuiWindowFlags& flags)
    : Window{"Liquid Status", width, height, flags} {}

LiquidStatusWindow::~LiquidStatusWindow() {}

void LiquidStatusWindow::show([[maybe_unused]] Manager* manager) {
  unsigned int status_id = 0;

  const ImVec2 size = util::size::h_wide(32.0f);
  const bool   active = true;

  ImGui::Columns(2, NULL, /* v_borders */ true);
  {
    if (ImGui::GetColumnIndex() == 0)
      ImGui::Separator();

    ImGui::Text("WATER");
    util::status_button("FULL", status_id++, active, size);
    util::status_button("NORMAL", status_id++, active, size);
    util::status_button("LOW", status_id++, active, size);
  }
  ImGui::NextColumn();
  {
    ImGui::Text("DISINFECTANT");
    util::status_button("FULL", status_id++, active, size);
    util::status_button("NORMAL", status_id++, active, size);
    util::status_button("LOW", status_id++, active, size);
  }
  ImGui::NextColumn();
}
}  // namespace gui

NAMESPACE_END
