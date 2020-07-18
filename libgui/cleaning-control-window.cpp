#include "gui.hpp"

#include "cleaning-control-window.hpp"

NAMESPACE_BEGIN

namespace gui {
CleaningControlWindow::CleaningControlWindow(float                   width,
                                             float                   height,
                                             const ImGuiWindowFlags& flags)
    : Window{"Cleaning Control", width, height, flags} {}

CleaningControlWindow::~CleaningControlWindow() {}

void CleaningControlWindow::show([[maybe_unused]] Manager* manager) {
  const ImVec2 size = util::size::h_wide(50.0f);
  unsigned int status_id = 0;

  bool active = false;

  ImGui::Columns(2, NULL, /* v_borders */ true);
  {
    ImGui::BeginChild("water_cleaning_control");
    ImGui::Separator();
    ImGui::Text("Water Cleaning Control");
    ImGui::Separator();
    ImGui::Text("Automatic Mode");
    if (util::button("1 Day", status_id++, active, size)) {
    }
    if (util::button("2 Day", status_id++, active, size)) {
    }
    if (util::button("3 Day", status_id++, active, size)) {
    }
    ImGui::Separator();
    ImGui::Text("Manual Mode");
    ImGui::Separator();
    if (util::button("Exchange Now", status_id++, active, size)) {
    }
    ImGui::EndChild();
  }
  ImGui::NextColumn();
  {
    ImGui::BeginChild("disinfectant_cleaning_control");
    ImGui::Separator();
    ImGui::Text("Disinfectant Cleaning Control");
    ImGui::Separator();
    ImGui::Text("Automatic Mode");
    if (util::button("1 Day", status_id++, active, size)) {
    }
    if (util::button("2 Day", status_id++, active, size)) {
    }
    if (util::button("3 Day", status_id++, active, size)) {
    }
    ImGui::Separator();
    ImGui::Text("Manual Mode");
    ImGui::Separator();
    if (util::button("Exchange Now", status_id++, active, size)) {
    }
    ImGui::EndChild();
  }
  ImGui::NextColumn();
}
}  // namespace gui

NAMESPACE_END
