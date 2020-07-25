#include "gui.hpp"

#include "movement-window.hpp"

#include <libmechanism/mechanism.hpp>

NAMESPACE_BEGIN

namespace gui {
MovementWindow::MovementWindow(float                   width,
                               float                   height,
                               const ImGuiWindowFlags& flags)
    : Window{"Movement", width, height, flags} {}

MovementWindow::~MovementWindow() {}

void MovementWindow::show([[maybe_unused]] Manager* manager) {
  massert(State::get() != nullptr, "sanity");
  massert(Config::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  auto*  state = State::get();
  auto&& movement = mechanism::movement_mechanism();

  ImGui::Columns(3, NULL, /* v_borders */ true);
  {
    if (ImGui::GetColumnIndex() == 0)
      ImGui::Separator();

    ImGui::Text("X");
    ImGui::Text("%f", state->x());
  }
  ImGui::NextColumn();
  {
    ImGui::Text("Y");
    ImGui::Text("%f", state->y());
  }
  ImGui::NextColumn();
  {
    ImGui::Text("Z");
    ImGui::Text("%f", state->z());
  }
  ImGui::NextColumn();
  ImGui::Separator();

  ImGui::Columns(1, NULL, /* v_borders */ true);
  // Typically we would use ImVec2(-1.0f,0.0f) or ImVec2(-FLT_MIN,0.0f) to use
  // all available width, or ImVec2(width,0.0f) for a specified width.
  // ImVec2(0.0f,0.0f) uses ItemWidth.
  ImGui::Text("Move Progress");
  ImGui::Separator();

  ImGui::PushID(0);
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram,
                        (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
  // ImGui::PushStyleColor(ImGuiCol_FrameRounding,
  //                       (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
  // ImGui::PushStyleColor(ImGuiCol_FrameBgActive,
  //                       (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
  ImGui::ProgressBar(movement->progress(), ImVec2(-FLT_MIN, 0.0f));
  ImGui::PopStyleColor(1);
  ImGui::PopID();
  // ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
}
}  // namespace gui

NAMESPACE_END
