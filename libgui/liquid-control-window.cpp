#include "gui.hpp"

#include "liquid-control-window.hpp"

#include <libmechanism/mechanism.hpp>

NAMESPACE_BEGIN

namespace gui {
LiquidControlWindow::LiquidControlWindow(machine::tending*       tsm,
                                         float                   width,
                                         float                   height,
                                         const ImGuiWindowFlags& flags)
    : Window{"Liquid Control", width, height, flags},
      tsm_{tsm},
      thread_pool_{2} {}

LiquidControlWindow::~LiquidControlWindow() {}

void LiquidControlWindow::show(Manager* manager) {
  massert(State::get() != nullptr, "sanity");
  massert(mechanism::LiquidRefilling::get() != nullptr, "sanity");

  auto* state = State::get();
  auto* liquid_refilling = mechanism::LiquidRefilling::get();

  const ImVec2 size = util::size::h_wide(50.0f);
  unsigned int status_id = 0;

  bool active = false;

  ImGui::Columns(2, NULL, /* v_borders */ true);
  {
    const bool disabled = !tsm()->is_no_task() || state->water_refilling();

    if (disabled) {
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
      ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }

    ImGui::BeginChild("water_control");
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
        thread_pool().enqueue(
            [liquid_refilling]() { liquid_refilling->exchange_water(); });
      }
      ImGui::PopFont();
    }
    ImGui::EndChild();

    if (disabled) {
      ImGui::PopItemFlag();
      ImGui::PopStyleVar();
    }
  }
  ImGui::NextColumn();
  {
    const bool disabled =
        !tsm()->is_no_task() || state->disinfectant_refilling();

    if (disabled) {
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
      ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }

    ImGui::BeginChild("disinfectant_control");
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
        thread_pool().enqueue([liquid_refilling]() {
          liquid_refilling->exchange_disinfectant();
        });
      }
      ImGui::PopFont();
    }
    ImGui::EndChild();

    if (disabled) {
      ImGui::PopItemFlag();
      ImGui::PopStyleVar();
    }
  }
  ImGui::NextColumn();
}
}  // namespace gui

NAMESPACE_END
