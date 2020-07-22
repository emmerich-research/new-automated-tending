#include "gui.hpp"

#include "fault-window.hpp"

#include "util.hpp"

NAMESPACE_BEGIN

namespace gui {

FaultWindow::FaultWindow(machine::tending*       tsm,
                         float                   width,
                         float                   height,
                         const ImGuiWindowFlags& flags)
    : Window{"Fault", width, height, flags}, tsm_{tsm} {}

FaultWindow::~FaultWindow() {}

void FaultWindow::show([[maybe_unused]] Manager* manager) {
  massert(State::get() != nullptr, "sanity");

  auto* state = State::get();

  const bool fault = state->fault();
  const bool manual_mode = state->manual_mode();

  const ImVec2 size = util::size::h_wide(50.0f);
  const ImVec2 popup_size = util::size::h_wide(125.0f);
  unsigned int id = 0;

  {
    if (!fault) {
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
      ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.8f);
    }

    if (util::button("Automatic Mode / Reset", id++, !fault, size)) {
      ImGui::OpenPopup("Reset?");
    }

    if (!fault) {
      ImGui::PopStyleVar();
      ImGui::PopItemFlag();
    }

    if (ImGui::BeginPopupModal("Reset?", NULL,
                               ImGuiWindowFlags_AlwaysAutoResize)) {
      ImGui::Text(
          "Please make sure that all hardware are \n"
          "configured correctly before \n"
          "going back to automatic mode!\n\n\n");
      ImGui::Separator();

      if (util::button("ACKNOWLEDGE", id++, false, popup_size)) {
        tsm()->restart();
        ImGui::CloseCurrentPopup();
      }
      ImGui::SetItemDefaultFocus();
      if (util::button("CANCEL", id++, false, popup_size)) {
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }
  }

  {
    if (fault) {
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
      ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.8f);
    }

    if (util::button("Fault Trigger", id++, fault, size)) {
      LOG_ERROR("[FAULT] fault trigger");
      state->fault(true);
      tsm()->fault();
    }

    if (fault) {
      ImGui::PopStyleVar();
      ImGui::PopItemFlag();
    }
  }

  {
    if (!fault || manual_mode) {
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
      ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.8f);
    }

    if (util::button("Manual Move", id++, manual_mode, size)) {
      tsm()->fault_manual();
    }

    if (!fault || manual_mode) {
      ImGui::PopStyleVar();
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    }
  }
}
}  // namespace gui

NAMESPACE_END
