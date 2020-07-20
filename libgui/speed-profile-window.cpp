#include "gui.hpp"

#include "speed-profile-window.hpp"

#include "util.hpp"

NAMESPACE_BEGIN

namespace gui {
SpeedProfileWindow::SpeedProfileWindow(const machine::tending* tsm,
                                       float                   width,
                                       float                   height,
                                       const ImGuiWindowFlags& flags)
    : Window{"Speed Profile", width, height, flags}, tsm_{tsm} {}

SpeedProfileWindow::~SpeedProfileWindow() {}

void SpeedProfileWindow::show([[maybe_unused]] Manager* manager) {
  massert(State::get() != nullptr, "sanity");

  auto*        state = State::get();
  unsigned int status_id = 0;
  const ImVec2 size = util::size::h_wide(50.0f);
  const auto&  current_speed = state->speed_profile();

  const bool disabled = !tsm()->is_no_task();

  if (disabled) {
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
  }

  ImGui::Columns(3, NULL, /* v_borders */ true);
  {
    const bool is_slow = current_speed == config::speed::slow;

    if (is_slow) {
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    }

    if (util::button("Slow", status_id++, is_slow, size)) {
      state->speed_profile(config::speed::slow);
    }

    if (is_slow) {
      ImGui::PopItemFlag();
    }
  }
  ImGui::NextColumn();
  {
    const bool is_normal = current_speed == config::speed::normal;

    if (is_normal) {
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    }

    if (util::button("Normal", status_id++, is_normal, size)) {
      state->speed_profile(config::speed::normal);
    }

    if (is_normal) {
      ImGui::PopItemFlag();
    }
  }
  ImGui::NextColumn();
  {
    const bool is_fast = current_speed == config::speed::fast;

    if (is_fast) {
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    }

    if (util::button("Fast", status_id++, is_fast, size)) {
      state->speed_profile(config::speed::fast);
    }

    if (is_fast) {
      ImGui::PopItemFlag();
    }
  }
  ImGui::NextColumn();

  if (disabled) {
    ImGui::PopItemFlag();
    ImGui::PopStyleVar();
  }
}
}  // namespace gui

NAMESPACE_END
