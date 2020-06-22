#include "precompiled.hpp"

#include <iostream>
#include <stdexcept>

#include <libcore/core.hpp>
#include <libdevice/device.hpp>
#include <libgui/gui.hpp>

#include "machine.hpp"

USE_NAMESPACE

static int throw_message() {
  std::cerr << "Failed to initialize machine, something is wrong" << std::endl;
  return ATM_ERR;
}

static void key_callback(GLFWwindow* window,
                         int         key,
                         int         scancode,
                         int         action,
                         int         mods) {
  if (mods == GLFW_MOD_ALT && key == GLFW_KEY_F4 && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  } else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

int main() {
  ATM_STATUS status = ATM_OK;
  // run state machine
  machine::tending tsm;

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

  // // main loop
  // while (!tsm.is_terminated()) {
  //   // sequences:
  //   // 1. spraying
  //   // 2. tending
  // }

  massert(window() != nullptr, "sanity");

  glfwSetKeyCallback(window(), key_callback);  // setup key callback

  // Our state
  bool   show_demo_window = true;
  bool   show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  while (!glfwWindowShouldClose(window())) {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
    // tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
    // your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
    // data to your main application. Generally you may always pass all inputs
    // to dear imgui, and hide them from your application based on those two
    // flags.
    glfwPollEvents();

    // Start the Dear ImGui frame
#if defined(OPENGL3_EXIST)
    ImGui_ImplOpenGL3_NewFrame();
#elif defined(OPENGL2_EXIST)
    ImGui_ImplOpenGL2_NewFrame();
#endif
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
      // draw control panel
      gui::control_panel_window();
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

#if defined(OPENGL3_EXIST)
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#elif defined(OPENGL2_EXIST)
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
#endif

    glfwSwapBuffers(window());
  }

  // stopping
  tsm.stop();
  massert(tsm.is_terminated(), "sanity");

  return ATM_OK;
}
