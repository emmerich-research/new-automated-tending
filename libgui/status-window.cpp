#include "gui.hpp"

#include "status-window.hpp"

#include "util.hpp"

NAMESPACE_BEGIN

namespace gui {
StatusWindow::StatusWindow(float                   width,
                           float                   height,
                           const ImGuiWindowFlags& flags)
    : Window{"Status", width, height, flags} {}

StatusWindow::~StatusWindow() {}

void StatusWindow::show([[maybe_unused]] Manager* manager) {
  massert(State::get() != nullptr, "sanity");

  auto* state = State::get();

  const ImVec2 size = util::size::h_wide(32.0f);
  unsigned int status_id = 0;

  ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.8f);

  ImGui::Columns(3, NULL, /* v_borders */ true);

  {
    // Spraying status
    if (ImGui::GetColumnIndex() == 0)
      ImGui::Separator();

    ImGui::Text("SPRAYING");
    util::status_button("READY", status_id++, state->spraying_ready(), size);
    util::status_button("RUNNING", status_id++, state->spraying_running(),
                        size);
    util::status_button("COMPLETE", status_id++, state->spraying_complete(),
                        size);
  }
  ImGui::NextColumn();
  {
    // Tending Status
    ImGui::Text("TENDING");
    util::status_button("READY", status_id++, state->tending_ready(), size);
    util::status_button("RUNNING", status_id++, state->tending_running(), size);
    util::status_button("COMPLETE", status_id++, state->tending_complete(),
                        size);
  }
  ImGui::NextColumn();
  {
    // Cleaning Status
    ImGui::Text("CLEANING");
    util::status_button("READY", status_id++, state->cleaning_ready(), size);
    util::status_button("RUNNING", status_id++, state->cleaning_running(),
                        size);
    util::status_button("COMPLETE", status_id++, state->cleaning_complete(),
                        size);
  }
  ImGui::NextColumn();

  ImGui::Columns(1);
  util::status_button("FAULT", status_id++, state->fault(), size);
  util::status_button("HOMING", status_id++, state->homing(), size);

  ImGui::PopStyleVar();
}
}  // namespace gui

NAMESPACE_END
