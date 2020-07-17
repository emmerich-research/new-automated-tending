#include "gui.hpp"

#include "manager.hpp"

#include <external/imgui/imconfig.h>
// #include <external/imgui/imstb_rectpack.h>
// #include <external/imgui/imstb_truetype.h>

#include <external/imgui/examples/imgui_impl_glfw.h>
#include <external/imgui/examples/imgui_impl_opengl3.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "window.hpp"

NAMESPACE_BEGIN

namespace gui {
Manager::Manager(ImVec4 clear_color, std::size_t num_window)
    : active_{true},
      exited_{false},
      num_window_{num_window},
      clear_color_{clear_color} {}

Manager::~Manager() {
  // Cleanup
  for (const auto* window : windows()) {
    delete window;
  }
  windows().clear();

  if (!exited()) {
    exit();
  }
}

void Manager::error_callback(const ErrorCallback&& error_cb) {
  glfwSetErrorCallback(error_cb);
}

void Manager::key_callback(const KeyCallback&& key_cb) {
  glfwSetKeyCallback(window(), key_cb);
}

void Manager::init(const char* name, int width, int height) {
  if (!glfwInit()) {
    active_ = false;
    return;
  }

// Decide GL+GLSL versions
#if __APPLE__
  // GL 3.2 + GLSL 150
  const char* glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // Required on Mac
#else
  // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
  // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif

  window_ = glfwCreateWindow(width, height, name, nullptr, nullptr);

  if (window() == nullptr) {
    active_ = false;
    return;
  }

  glfwMakeContextCurrent(window());
  glfwSwapInterval(1);  // Enable vsync

  if (gladLoadGL() == 0) {
    active_ = false;
    return;
  }

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
  // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window(), true);

  ImGui_ImplOpenGL3_Init(glsl_version);

  ImFontConfig font_config;
  font_config.SizePixels = 25.0f;

  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->AddFontDefault(&font_config);
}

void Manager::exit() {
  if (active()) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window());
    glfwTerminate();
  }
  exited_ = true;
}

bool Manager::handle_events() {
  bool should_not_close = !glfwWindowShouldClose(window());

  if (should_not_close) {
    glfwPollEvents();
  }

  should_not_close = !glfwWindowShouldClose(window());

  return should_not_close;
}

void Manager::add_window(Window* window) {
  windows().push_back(window);
}

void Manager::render() {
#if defined(OPENGL3_EXIST)
  ImGui_ImplOpenGL3_NewFrame();
#elif defined(OPENGL2_EXIST)
  ImGui_ImplOpenGL2_NewFrame();
#endif

  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  {
    ImGui::Begin("Panel");

    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);

    ImGui::End();
  }

  // render windows
  for (auto* s_window : windows())
    s_window->render(reinterpret_cast<const Manager*>(this));

  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window(), &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(clear_color().x, clear_color().y, clear_color().z,
               clear_color().w);
  glClear(GL_COLOR_BUFFER_BIT);

#if defined(OPENGL3_EXIST)
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#elif defined(OPENGL2_EXIST)
  ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
#endif

  glfwSwapBuffers(window());

  // after render windows
  for (auto* s_window : windows())
    s_window->after_render(reinterpret_cast<const Manager*>(this));
}
}  // namespace gui

NAMESPACE_END
