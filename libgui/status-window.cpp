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

void StatusWindow::show([[maybe_unused]] const Manager* manager) {
  massert(State::get() != nullptr, "sanity");

  auto* state = State::get();

  const ImVec2 size = util::size::h_wide(50.0f);
  unsigned int status_id = 0;

  ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.8f);

  ImGui::Columns(3, NULL, /* v_borders */ true);

  {
    // Spraying status
    if (ImGui::GetColumnIndex() == 0)
      ImGui::Separator();

    ImGui::Text("Spraying Status");
    util::status_button("Spraying Ready", status_id++, state->spraying_ready(),
                        size);
    util::status_button("Spraying Running", status_id++,
                        state->spraying_running(), size);
    util::status_button("Spraying Complete", status_id++,
                        state->spraying_complete(), size);
    // util::status_button("Spraying Fault", status_id++,
    // state->spraying_fault(),
    //                     size);
  }
  ImGui::NextColumn();
  {
    // Tending Status
    ImGui::Text("Tending Status");
    util::status_button("Tending Ready", status_id++, state->tending_ready(),
                        size);
    util::status_button("Tending Running", status_id++,
                        state->tending_running(), size);
    util::status_button("Tending Complete", status_id++,
                        state->tending_complete(), size);
    // util::status_button("Tending Fault", status_id++, state->tending_fault(),
    //                     size);
  }
  ImGui::NextColumn();
  {
    // Cleaning Status
    ImGui::Text("Cleaning Status");
    util::status_button("Cleaning Ready", status_id++, state->tending_ready(),
                        size);
    util::status_button("Cleaning Running", status_id++,
                        state->tending_running(), size);
    util::status_button("Cleaning Complete", status_id++,
                        state->tending_complete(), size);
    // util::status_button("Cleaning Fault", status_id++,
    // state->tending_fault(),
    //                     size);
  }
  ImGui::NextColumn();

  ImGui::Columns(1);
  util::status_button("Fault", status_id++, state->fault(), size);

  ImGui::PopStyleVar();
}
}  // namespace gui

NAMESPACE_END
