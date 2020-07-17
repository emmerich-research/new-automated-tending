#include "gui.hpp"

#include "cleaning-window.hpp"

#include "util.hpp"

NAMESPACE_BEGIN

namespace gui {

CleaningWindow::CleaningWindow(float                   width,
                               float                   height,
                               const ImGuiWindowFlags& flags)
    : Window{"Cleaning", width, height, flags} {}

CleaningWindow::~CleaningWindow() {}

void CleaningWindow::show([[maybe_unused]] const Manager* manager) {
  unsigned int status_id = 0;

  const ImVec2 size = util::size::h_wide(50.0f);
  const bool   active = true;

  ImGui::Columns(2, NULL, /* v_borders */ true);
  {
    util::status_button("Water Full", status_id++, active, size);
    util::status_button("Water Normal", status_id++, active, size);
    util::status_button("Water Low", status_id++, active, size);
    ImGui::NextColumn();
  }

  {
    util::status_button("Disinfectant Full", status_id++, active, size);
    util::status_button("Disinfectant Normal", status_id++, active, size);
    util::status_button("Disinfectant Low", status_id++, active, size);
    ImGui::NextColumn();
  }
}
}  // namespace gui

NAMESPACE_END
