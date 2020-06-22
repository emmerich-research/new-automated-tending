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

  const ImGuiStyle& style = ImGui::GetStyle();

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

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair
    // to created a named window.
    {
      static float f = 0.0f;
      static int   counter = 0;

      gui::reset_window_pos();

      auto [width, height] = gui::get_window_size();

      ImGui::SetNextWindowSize(
          ImVec2{static_cast<float>(width), static_cast<float>(height)});
      // ImGui::SetNextWindowContentSize(ImVec2{window_width, window_height});

      ImGui::Begin("Control Panel");  // Create a window called "Hello, world!"
                                      // and append into it.

      static bool h_borders = true;
      static bool v_borders = true;
      static int  top_columns_count = 2;
      static int  bottom_columns_count = 3;

      // ImGui::BeginGroup();
      // {
      //   ImGui::BeginGroup();
      //   ImGui::Button("AAA");
      //   ImGui::SameLine();
      //   ImGui::Button("BBB");
      //   ImGui::SameLine();
      //   ImGui::BeginGroup();
      //   ImGui::Button("CCC");
      //   ImGui::Button("DDD");
      //   ImGui::EndGroup();
      //   ImGui::SameLine();
      //   ImGui::Button("EEE");
      //   ImGui::EndGroup();
      //   if (ImGui::IsItemHovered())
      //     ImGui::SetTooltip("First group hovered");
      // }
      // ImGui::EndGroup();

      // Top Row
      ImGui::Columns(top_columns_count, NULL, v_borders);
      {
        ImGui::BeginChild("inner_status");
        ImGui::Columns(2, NULL, v_borders);
        {
          // Spraying status
          if (h_borders && ImGui::GetColumnIndex() == 0)
            ImGui::Separator();

          ImGui::Text("Spraying Status");
          // ImGui::Text("Width %.2f", ImGui::GetColumnWidth());
          // ImGui::Text("Avail %.2f", ImGui::GetContentRegionAvail().x);
          // ImGui::Text("Offset %.2f", ImGui::GetColumnOffset());
          // ImGui::Text("Long text that is likely to clip");
          // ImGui::Button("Button", ImVec2(-FLT_MIN, 0.0f));
          ImGui::PushID(0);
          ImGui::PushStyleColor(ImGuiCol_Button,
                                (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
          ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                                (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
          ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                (ImVec4)ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
          ImGui::Button("Spray Ready", ImVec2(-FLT_MIN, 0.0f));
          ImGui::PopStyleColor(3);
          ImGui::PopID();
          ImGui::NextColumn();
        }

        {
          // Tending Status
          ImGui::Text("Tending Status");
          ImGui::PushID(0);
          ImGui::PushStyleColor(ImGuiCol_Button,
                                (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
          ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                                (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
          ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                (ImVec4)ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
          ImGui::Button("Tending Ready", ImVec2(-FLT_MIN, 0.0f));
          ImGui::PopStyleColor(3);
          ImGui::PopID();
          ImGui::NextColumn();
        }
        ImGui::EndChild();
      }

      {
        // Manual Movement

        const ImVec2 button_size{100, 100};
        ImGui::NextColumn();

        ImGui::BeginChild("inner_movement");
        ImGui::Columns(2, NULL, false);
        {
          ImGui::BeginGroup();
          {
            ImGui::Separator();
            ImGui::SameLine(button_size.x + style.FramePadding.x * 2);
            ImGui::Button("Y+", button_size);
            {
              ImGui::BeginGroup();
              ImGui::Button("X-", button_size);
              ImGui::SameLine();
              ImGui::Button("HOME", button_size);
              ImGui::SameLine();
              ImGui::Button("X+", button_size);
              ImGui::EndGroup();
            }
            ImGui::SetCursorPosX(ImGui::GetColumnOffset() + button_size.x +
                                 style.FramePadding.x * 4);
            ImGui::Button("Y-", button_size);
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::NextColumn();
        }

        {
          ImGui::SetCursorPosY(style.FramePadding.y * 20);
          ImGui::Button("Z+", button_size);
          ImGui::Button("Z-", button_size);
          ImGui::NextColumn();
        }
        ImGui::EndChild();
      }

      // Bottom Row
      ImGui::Columns(1);
      if (h_borders)
        ImGui::Separator();

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window) {
      ImGui::Begin(
          "Another Window",
          &show_another_window);  // Pass a pointer to our bool variable (the
                                  // window will have a closing button that will
                                  // clear the bool when clicked)
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close Me"))
        show_another_window = false;
      ImGui::End();
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
