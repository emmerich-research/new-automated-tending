#include "gui.hpp"

#include "manual-movement-window.hpp"

#include <libmechanism/mechanism.hpp>

NAMESPACE_BEGIN

namespace gui {

ManualMovementWindow::ManualMovementWindow(machine::tending*       tsm,
                                           float                   width,
                                           float                   height,
                                           const ImGuiWindowFlags& flags)
    : Window{"Manual Movement", width, height, flags},
      tsm_{tsm},
      thread_pool_{1} {}

ManualMovementWindow::~ManualMovementWindow() {}

void ManualMovementWindow::show([[maybe_unused]] Manager* manager) {
  massert(State::get() != nullptr, "sanity");
  massert(Config::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  // Manual Movement
  auto*       state = State::get();
  const auto* config = Config::get();
  auto&&      movement = mechanism::movement_mechanism();

  const ImGuiStyle& style = ImGui::GetStyle();
  const ImVec2      button_size{75, 75};

  const bool manual_mode = state->manual_mode();

  const double x_manual = config->fault_manual_movement<double>("x");
  const double y_manual = config->fault_manual_movement<double>("y");
  const double z_manual = config->fault_manual_movement<double>("z");

  const bool disabled = !manual_mode || !movement->ready();

  if (disabled) {
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
  }

  ImGui::Columns(2, NULL, false);
  {
    ImGui::BeginGroup();
    {
      ImGui::Separator();
      ImGui::SameLine(button_size.x + style.FramePadding.x * 2);
      if (ImGui::Button("Y+", button_size)) {
        thread_pool().enqueue([=]() {
          movement->move<mechanism::movement::unit::mm>(0.0, y_manual, 0.0);
        });
      }
      {
        ImGui::BeginGroup();
        if (ImGui::Button("X-", button_size)) {
          thread_pool().enqueue([=]() {
            movement->move<mechanism::movement::unit::mm>(-x_manual, 0.0, 0.0);
          });
        }
        ImGui::SameLine();
        if (ImGui::Button("HOME", button_size)) {
          thread_pool().enqueue([=]() { movement->homing(); });
        }
        ImGui::SameLine();
        if (ImGui::Button("X+", button_size)) {
          thread_pool().enqueue([=]() {
            movement->move<mechanism::movement::unit::mm>(x_manual, 0.0, 0.0);
          });
        }
        ImGui::EndGroup();
      }
      ImGui::SetCursorPosX(ImGui::GetColumnOffset() + button_size.x +
                           style.FramePadding.x * 4);
      if (ImGui::Button("Y-", button_size)) {
        thread_pool().enqueue([=]() {
          movement->move<mechanism::movement::unit::mm>(0.0, -y_manual, 0.0);
        });
      }

      ImGui::EndGroup();
    }
    ImGui::EndGroup();

    ImGui::NextColumn();
  }

  {
    ImGui::SetCursorPosX(ImGui::GetColumnOffset() + button_size.x +
                         style.FramePadding.x * 4);
    ImGui::SetCursorPosY(style.FramePadding.y * 20);
    if (ImGui::Button("Z+", button_size)) {
      thread_pool().enqueue([=]() {
        movement->move<mechanism::movement::unit::mm>(0.0, 0.0, z_manual);
      });
    }

    ImGui::SetCursorPosX(ImGui::GetColumnOffset() + button_size.x +
                         style.FramePadding.x * 4);
    if (ImGui::Button("Z-", button_size)) {
      thread_pool().enqueue([=]() {
        movement->move<mechanism::movement::unit::mm>(0.0, 0.0, -z_manual);
      });
    }
    ImGui::NextColumn();
  }

  if (disabled) {
    ImGui::PopItemFlag();
    ImGui::PopStyleVar();
  }
}
}  // namespace gui

NAMESPACE_END
