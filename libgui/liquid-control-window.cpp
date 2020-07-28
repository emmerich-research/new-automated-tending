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
    const bool disabled =
        !tsm()->is_no_task() || state->water_refilling_running();
    const auto& schedule = state->water_refilling_schedule();

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
      if (util::button("1 DAY", status_id++, schedule == Refill::ONE_DAY,
                       size)) {
        LOG_INFO("Changing water refilling to one day");
        state->water_refilling_schedule(Refill::ONE_DAY);
      }
      if (util::button("2 DAYS", status_id++, schedule == Refill::TWO_DAYS,
                       size)) {
        LOG_INFO("Changing water refilling to two days");
        state->water_refilling_schedule(Refill::TWO_DAYS);
      }
      if (util::button("3 DAYS", status_id++, schedule == Refill::THREE_DAYS,
                       size)) {
        LOG_INFO("Changing water refilling to three days");
        state->water_refilling_schedule(Refill::THREE_DAYS);
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
        !tsm()->is_no_task() || state->disinfectant_refilling_running();
    const auto& schedule = state->disinfectant_refilling_schedule();

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
      if (util::button("1 DAY", status_id++, schedule == Refill::ONE_DAY,
                       size)) {
        LOG_INFO("Changing water refilling to one day");
        state->disinfectant_refilling_schedule(Refill::ONE_DAY);
      }
      if (util::button("2 DAYS", status_id++, schedule == Refill::TWO_DAYS,
                       size)) {
        LOG_INFO("Changing water refilling to two days");
        state->disinfectant_refilling_schedule(Refill::TWO_DAYS);
      }
      if (util::button("3 DAYS", status_id++, schedule == Refill::THREE_DAYS,
                       size)) {
        LOG_INFO("Changing water refilling to three days");
        state->disinfectant_refilling_schedule(Refill::THREE_DAYS);
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
