#include "gui.hpp"

#include "plc-trigger-window.hpp"

#include <libdevice/device.hpp>

#include "util.hpp"

NAMESPACE_BEGIN

namespace gui {

PLCTriggerWindow::PLCTriggerWindow(float                   width,
                                   float                   height,
                                   const ImGuiWindowFlags& flags)
    : Window{"PLC Trigger", width, height, flags} {}

PLCTriggerWindow::~PLCTriggerWindow() {}

void PLCTriggerWindow::show([[maybe_unused]] Manager* manager) {
  massert(device::DigitalInputDeviceRegistry::get() != nullptr, "sanity");

  auto*  input_registry = device::DigitalInputDeviceRegistry::get();
  auto&& spraying_tending_height =
      input_registry->get(device::id::comm::plc::spraying_tending_height());
  auto&& cleaning_height =
      input_registry->get(device::id::comm::plc::cleaning_height());

  const ImVec2 size{-FLT_MIN, 32.0f};
  unsigned int status_id = 0;

  ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.8f);

  ImGui::Columns(2, NULL, /* v_borders */ true);
  util::status_button("SPRAYING / TENDING", status_id++,
                      spraying_tending_height->read_bool(), size);
  ImGui::NextColumn();
  util::status_button("CLEANING", status_id++, cleaning_height->read_bool(),
                      size);
  ImGui::NextColumn();

  ImGui::PopStyleVar();
}
}  // namespace gui

NAMESPACE_END

