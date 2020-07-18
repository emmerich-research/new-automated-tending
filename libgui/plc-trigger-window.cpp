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

  const ImVec2 size{288.5f, 50.0f};
  unsigned int status_id = 0;

  util::status_button("Spraying/Tending", status_id++,
                      spraying_tending_height->read_bool(), size);
  ImGui::SameLine();
  util::status_button("Cleaning", status_id++, cleaning_height->read_bool(),
                      size);
}
}  // namespace gui

NAMESPACE_END

