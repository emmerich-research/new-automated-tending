#include "gui.hpp"

#include "cleaning-control-window.hpp"

NAMESPACE_BEGIN

namespace gui {
CleaningControlWindow::CleaningControlWindow(float                   width,
                                             float                   height,
                                             const ImGuiWindowFlags& flags)
    : Window{"Cleaning Control", width, height, flags} {}

CleaningControlWindow::~CleaningControlWindow() {}

void CleaningControlWindow::show(Manager* manager) {
  const ImVec2 size = util::size::h_wide(50.0f);
  unsigned int status_id = 0;

  bool active = false;

  ImGui::Columns(2, NULL, /* v_borders */ true);
  {
    ImGui::BeginChild("water_cleaning_control");
    ImGui::Separator();
    ImGui::Text("Water");
    ImGui::Separator();
    ImGui::Text("Automatic Mode");
    {
      ImGui::PushFont(manager->button_font());
      if (util::button("1 DAY", status_id++, active, size)) {
      }
      if (util::button("2 DAYS", status_id++, active, size)) {
      }
      if (util::button("3 DAYS", status_id++, active, size)) {
      }
      ImGui::PopFont();
    }
    ImGui::Separator();
    ImGui::Text("Manual Mode");
    ImGui::Separator();
    {
      ImGui::PushFont(manager->button_font());
      if (util::button("EXCHANGE NOW", status_id++, active, size)) {
      }     
      ImGui::PopFont();
    }
    ImGui::EndChild();
  }
  ImGui::NextColumn();
  {
    ImGui::BeginChild("disinfectant_cleaning_control");
    ImGui::Separator();
    ImGui::Text("Disinfectant");
    ImGui::Separator();
    ImGui::Text("Automatic Mode");
    {
      ImGui::PushFont(manager->button_font());
      if (util::button("1 DAY", status_id++, active, size)) {
      }
      if (util::button("2 DAYS", status_id++, active, size)) {
      }
      if (util::button("3 DAYS", status_id++, active, size)) {
      }
      ImGui::PopFont();
    }
    ImGui::Separator();
    ImGui::Text("Manual Mode");
    ImGui::Separator();
    {
      ImGui::PushFont(manager->button_font());
      if (util::button("EXCHANGE NOW", status_id++, active, size)) {
      }     
      ImGui::PopFont();
    }
    ImGui::EndChild();
  }
  ImGui::NextColumn();
}
}  // namespace gui

NAMESPACE_END
