#include "gui.hpp"

#include "util.hpp"

NAMESPACE_BEGIN

namespace gui {
namespace util {
namespace size {
ImVec2 h_wide(float height) {
  return {-FLT_MIN, height};
}
}  // namespace size

bool button(const char*   label,
            unsigned int  status_id,
            bool          active,
            const ImVec2& size) {
  bool result = false;

  if (active) {
    ImGui::PushID(status_id);

    // green
    ImGui::PushStyleColor(ImGuiCol_Button,
                          (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
  }

  result = ImGui::Button(label, size);

  if (active) {
    ImGui::PopStyleColor(3);
    ImGui::PopID();
  }

  return result;
}

void status_button(const char*   label,
                   unsigned int  status_id,
                   bool          active,
                   const ImVec2& size) {
  ImGui::PushID(status_id);
  ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);

  if (active) {
    // green
    ImGui::PushStyleColor(ImGuiCol_Button,
                          (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
  } else {
    // red
    ImGui::PushStyleColor(ImGuiCol_Button,
                          (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          (ImVec4)ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
  }

  ImGui::Button(label, size);
  ImGui::PopStyleColor(3);
  ImGui::PopItemFlag();
  ImGui::PopID();
}

}  // namespace util
}  // namespace gui

NAMESPACE_END
