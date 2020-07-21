#include <iostream>
#include <stdexcept>

#include <libcore/core.hpp>
#include <libdevice/device.hpp>
#include <libgui/gui.hpp>
#include <libmachine/machine.hpp>

int main() {
  USE_NAMESPACE;

  ATM_STATUS             status = ATM_OK;
  machine::tending       tsm;
  gui::Manager           ui_manager;
  machine::FaultListener fault_listener(&tsm);

  // initialization
  try {
    tsm.start();
    massert(tsm.is_running(), "sanity");
  } catch (std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    status = ATM_ERR;
  }

  // early stopping
  if (status == ATM_ERR) {
    tsm.stop();
    massert(tsm.is_terminated(), "sanity");
    return status;
  }

  fault_listener.start();

  ui_manager.name(Config::get()->name());
  ui_manager.init();

  // early stopping
  if (!ui_manager.active()) {
    return ATM_ERR;
  }

  ui_manager.key_callback([](gui::Manager::MainWindow* current_window, int key,
                             [[maybe_unused]] int scancode, int action,
                             int mods) {
    if (mods == GLFW_MOD_ALT && key == GLFW_KEY_F4 && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(current_window, GL_TRUE);
    } else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(current_window, GL_TRUE);
    }
  });

  ui_manager.error_callback([](int error, const char* description) {
    LOG_ERROR("Glfw Error {}: {}", error, description);
  });

  ui_manager.add_window<gui::SystemInfoWindow>();
  ui_manager.add_window<gui::FaultWindow>(&tsm);
  ui_manager.add_window<gui::MovementWindow>();
  ui_manager.add_window<gui::ManualMovementWindow>(&tsm);
  ui_manager.add_window<gui::StatusWindow>();
  ui_manager.add_window<gui::CleaningWindow>();
  ui_manager.add_window<gui::CleaningControlWindow>();
  ui_manager.add_window<gui::PLCTriggerWindow>();
  ui_manager.add_window<gui::SpeedProfileWindow>(
      reinterpret_cast<const machine::tending*>(&tsm));

  while (ui_manager.handle_events()) {
    ui_manager.render();
  }

  // stopping
  ui_manager.exit();
  fault_listener.stop();
  State::get()->fault(true);
  LOG_INFO("Killing task workers, will go into fault mode to kill app...");
  tsm.fault();
  sleep_for<time_units::seconds>(5);
  tsm.stop();
  massert(tsm.is_terminated(), "sanity");

  return ATM_OK;
}
