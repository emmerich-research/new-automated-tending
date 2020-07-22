#include "gui.hpp"

#include "manual-movement-window.hpp"

#include <libmechanism/mechanism.hpp>

#include "util.hpp"

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

  const ImVec2 button_size = util::size::h_wide(75);

  const double x_manual = config->fault_manual_movement<double>("x");
  const double y_manual = config->fault_manual_movement<double>("y");
  const double z_manual = config->fault_manual_movement<double>("z");

  const bool disabled = !state->manual_mode() || !movement->ready();

  if (disabled) {
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
  }

  ImGui::Columns(3, NULL, false);
  {
    if (ImGui::GetColumnIndex() == 0)
      ImGui::Separator();

    if (ImGui::Button("X+", button_size)) {
      thread_pool().enqueue([this, x_manual]() {
        move<mechanism::movement::unit::mm>(x_manual, 0.0, 0.0);
      });
    }

    if (ImGui::Button("X-", button_size)) {
      thread_pool().enqueue([this, x_manual]() {
        move<mechanism::movement::unit::mm>(-x_manual, 0.0, 0.0);
      });
    }
  }
  ImGui::NextColumn();
  {
    if (ImGui::Button("Y+", button_size)) {
      thread_pool().enqueue([this, y_manual]() {
        move<mechanism::movement::unit::mm>(0.0, y_manual, 0.0);
      });
    }

    if (ImGui::Button("Y-", button_size)) {
      thread_pool().enqueue([this, y_manual]() {
        move<mechanism::movement::unit::mm>(0.0, -y_manual, 0.0);
      });
    }
  }
  ImGui::NextColumn();
  {
    if (ImGui::Button("Z+", button_size)) {
      thread_pool().enqueue([this, z_manual]() {
        move<mechanism::movement::unit::mm>(0.0, 0.0, z_manual);
      });
    }

    if (ImGui::Button("Z-", button_size)) {
      thread_pool().enqueue([this, z_manual]() {
        move<mechanism::movement::unit::mm>(0.0, 0.0, -z_manual);
      });
    }
  }
  ImGui::NextColumn();

  ImGui::Columns(1);
  ImGui::Separator();
  if (ImGui::Button("HOME", button_size)) {
    thread_pool().enqueue([movement]() mutable { movement->homing(); });
  }

  if (disabled) {
    ImGui::PopItemFlag();
    ImGui::PopStyleVar();
  }
}
}  // namespace gui

NAMESPACE_END
