#include "gui.hpp"

#include "widget.hpp"

// #include <functional>

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace gui {
namespace widget {
static void status_button(const char* label, unsigned int status_id, bool active
                          /* const std::function<void()>& callback */) {
  ImGui::PushID(status_id);

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

  // callback();
  [[maybe_unused]] bool result = ImGui::Button(label, ImVec2(-FLT_MIN, 50.0f));
  ImGui::PopStyleColor(3);
  ImGui::PopID();
}

void status() {
  massert(State::get() != nullptr, "sanity");

  auto* state = State::get();

  {
    ImGui::BeginChild("inner_status");

    ImGui::Columns(2, NULL, /* v_borders */ true);
    unsigned int status_id = 0;
    {
      // Spraying status
      if (ImGui::GetColumnIndex() == 0)
        ImGui::Separator();

      ImGui::Text("Spraying Status");
      status_button("Spraying Ready", status_id++, state->spraying_ready());
      status_button("Spraying Running", status_id++, state->spraying_running());
      status_button("Spraying Complete", status_id++,
                    state->spraying_complete());
      status_button("Spraying Fault", status_id++, state->spraying_fault());
      ImGui::NextColumn();
    }

    {
      // Tending Status
      ImGui::Text("Tending Status");
      status_button("Tending Ready", status_id++, state->tending_ready());
      status_button("Tending Running", status_id++, state->tending_running());
      status_button("Tending Complete", status_id++, state->tending_complete());
      status_button("Tending Fault", status_id++, state->tending_fault());
      ImGui::NextColumn();
    }

    cleaning();

    ImGui::EndChild();
  }
}

void manual() {
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
  const bool fault = state->fault();

  const double x_manual = config->fault_manual_movement<double>("x");
  const double y_manual = config->fault_manual_movement<double>("y");
  const double z_manual = config->fault_manual_movement<double>("z");

  // state->reset_coordinate();

  if (!manual_mode) {
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
  }

  ImGui::BeginChild("inner_movement");
  ImGui::Columns(2, NULL, false);
  {
    ImGui::BeginGroup();
    {
      ImGui::Separator();
      ImGui::SameLine(button_size.x + style.FramePadding.x * 2);
      if (ImGui::Button("Y+", button_size)) {
        movement->move<mechanism::movement::unit::mm>(0.0, y_manual, 0.0);
      }
      {
        ImGui::BeginGroup();
        if (ImGui::Button("X-", button_size)) {
          movement->move<mechanism::movement::unit::mm>(-x_manual, 0.0, 0.0);
        }
        ImGui::SameLine();
        if (ImGui::Button("HOME", button_size)) {
          movement->homing();
        }
        ImGui::SameLine();
        if (ImGui::Button("X+", button_size)) {
          movement->move<mechanism::movement::unit::mm>(x_manual, 0.0, 0.0);
        }
        ImGui::EndGroup();
      }
      ImGui::SetCursorPosX(ImGui::GetColumnOffset() + button_size.x +
                           style.FramePadding.x * 4);
      if (ImGui::Button("Y-", button_size)) {
        movement->move<mechanism::movement::unit::mm>(0.0, -y_manual, 0.0);
      }
      {
        // Fault
        if (fault) {
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
          ImGui::PushStyleVar(ImGuiStyleVar_Alpha,
                              ImGui::GetStyle().Alpha * 0.5f);
        } else {
          ImGui::PopItemFlag();
          ImGui::PopStyleVar();
        }
        ImGui::Separator();
        if (ImGui::Button("Fault Test", ImVec2{-FLT_MIN, 100})) {
        }
        if (fault) {
          ImGui::PopItemFlag();
          ImGui::PopStyleVar();
        } else {
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
          ImGui::PushStyleVar(ImGuiStyleVar_Alpha,
                              ImGui::GetStyle().Alpha * 0.5f);
        }
        if (ImGui::Button("Manual Move", ImVec2{-FLT_MIN, 100})) {
        }
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
      movement->move<mechanism::movement::unit::mm>(0.0, 0.0, z_manual);
    }

    ImGui::SetCursorPosX(ImGui::GetColumnOffset() + button_size.x +
                         style.FramePadding.x * 4);
    if (ImGui::Button("Z-", button_size)) {
      movement->move<mechanism::movement::unit::mm>(0.0, 0.0, -z_manual);
    }
    ImGui::NextColumn();
  }

  if (!manual_mode) {
    ImGui::PopItemFlag();
    ImGui::PopStyleVar();
  }

  ImGui::EndChild();

  // state->reset_coordinate();
}

void cleaning() {
  // ImGui::BeginChild("inner_cleaning");
  {
    ImGui::BeginGroup();
    {
      ImGui::Button("W Full", ImVec2{ImGui::GetColumnWidth() / 2, 100});
      ImGui::Button("W Low", ImVec2{ImGui::GetColumnWidth() / 2, 100});
    }
    ImGui::EndGroup();
  }
  ImGui::SameLine();
  {
    ImGui::BeginGroup();
    {
      ImGui::Button("D Full", ImVec2{ImGui::GetColumnWidth() / 2, 100});
      ImGui::Button("D Low", ImVec2{ImGui::GetColumnWidth() / 2, 100});
    }
    ImGui::EndGroup();
  }
  ImGui::NextColumn();
  {
    ImGui::BeginGroup();
    {
      ImGui::Button("WE Auto", ImVec2{ImGui::GetColumnWidth() / 2, 100});
      ImGui::Button("WE Manual", ImVec2{ImGui::GetColumnWidth() / 2, 100});
    }
    ImGui::EndGroup();
  }
  ImGui::SameLine();
  {
    ImGui::BeginGroup();
    {
      ImGui::Button("DE Auto", ImVec2{ImGui::GetColumnWidth() / 2, 100});
      ImGui::Button("DE Manual", ImVec2{ImGui::GetColumnWidth() / 2, 100});
    }
    ImGui::EndGroup();
  }
  // {
  //   ImGui::Text("Fluid Level Status");
  //   {
  //     ImGui::BeginGroup();
  //     {
  //       ImGui::Button("W Full", ImVec2{ImGui::GetColumnWidth() / 2, 100});
  //       ImGui::Button("W Low", ImVec2{ImGui::GetColumnWidth() / 2, 100});
  //     }
  //     ImGui::EndGroup();
  //   }
  //   ImGui::SameLine();
  //   {
  //     ImGui::BeginGroup();
  //     {
  //       ImGui::Button("D Full", ImVec2{ImGui::GetColumnWidth() / 2, 100});
  //       ImGui::Button("D Low", ImVec2{ImGui::GetColumnWidth() / 2, 100});
  //     }
  //     ImGui::EndGroup();
  //   }

  //   ImGui::NextColumn();
  // }

  // {
  //   ImGui::Text("Exchange Water");
  //   {
  //     ImGui::Button("1 Day", ImVec2{ImGui::GetColumnWidth() / 3, 100});
  //     ImGui::SameLine();
  //     ImGui::Button("2 Day", ImVec2{ImGui::GetColumnWidth() / 3, 100});
  //     ImGui::SameLine();
  //     ImGui::Button("3 Day", ImVec2{ImGui::GetColumnWidth() / 3, 100});
  //     ImGui::Button("W Manual Exchange", ImVec2{-FLT_MIN, 100});
  //   }
  //   ImGui::NextColumn();
  // }

  // {
  //   ImGui::Text("Exchange Disinfectant");
  //   {
  //     ImGui::Button("1 Day", ImVec2{ImGui::GetColumnWidth() / 3, 100});
  //     ImGui::SameLine();
  //     ImGui::Button("2 Day", ImVec2{ImGui::GetColumnWidth() / 3, 100});
  //     ImGui::SameLine();
  //     ImGui::Button("3 Day", ImVec2{ImGui::GetColumnWidth() / 3, 100});
  //     ImGui::Button("D Manual Exchange", ImVec2{-FLT_MIN, 100});
  //   }
  //   ImGui::NextColumn();
  // }
}
}  // namespace widget
}  // namespace gui

NAMESPACE_END
