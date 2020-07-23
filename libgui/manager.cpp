#include "gui.hpp"

#include "manager.hpp"

#include <external/imgui/imconfig.h>

#include <external/imgui/examples/imgui_impl_glfw.h>

#if defined(OPENGL3_EXIST)
#include <external/imgui/examples/imgui_impl_opengl3.h>

#include <glad/glad.h>
#elif defined(OPENGL2_EXIST)
#include <external/imgui/examples/imgui_impl_opengl2.h>

#include <GL/gl.h>
#endif

#include <GLFW/glfw3.h>

#include <libutil/util.hpp>

NAMESPACE_BEGIN

namespace gui {
Manager::Manager(const std::string& name, ImVec4 clear_color)
    : name_{name},
      active_{true},
      terminated_{false},
      exited_{false},
      clear_color_{clear_color},
      window_{nullptr},
      general_font_{nullptr},
      button_font_{nullptr},
      logging_font_{nullptr} {}

Manager::~Manager() {
  // Cleanup
  // for (const auto* window : windows()) {
  //   delete window;
  // }
  windows().clear();

  if (!exited()) {
    exit();
  }
}

void Manager::name(const char* name) {
  name_ = name;
}

void Manager::name(const std::string& name) {
  name_ = name;
}

void Manager::clear_color(const ImVec4& color) {
  clear_color_ = color;
}

void Manager::error_callback(const ErrorCallback&& error_cb) {
  glfwSetErrorCallback(error_cb);
}

void Manager::key_callback(const KeyCallback&& key_cb) {
  glfwSetKeyCallback(window(), key_cb);
}

void Manager::init() {
  if (!glfwInit()) {
    active_ = false;
    return;
  }

#if defined(OPENGL3_EXIST)
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
#endif

  auto*              monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

  window_ = glfwCreateWindow(mode->width, mode->height, name().c_str(), nullptr,
                             nullptr);

  glfwSetWindowMonitor(window(), monitor, 0, 0, mode->width, mode->height,
                       mode->refreshRate);

  if (window() == nullptr) {
    active_ = false;
    return;
  }

  glfwMakeContextCurrent(window());
  glfwSwapInterval(1);  // Enable vsync

#if defined(OPENGL3_EXIST)
  if (gladLoadGL() == 0) {
    active_ = false;
    return;
  }
#endif

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
  // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();
  // ImGui::StyleColorsClassic();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window(), true);

#if defined(OPENGL3_EXIST)
  ImGui_ImplOpenGL3_Init(glsl_version);
#elif defined(OPENGL2_EXIST)
  ImGui_ImplOpenGL2_Init();
#endif

  ImGuiIO& io = ImGui::GetIO();

  // if (fs::is_regular_file("fonts/cousine.ttf")) {
  general_font_ = io.Fonts->AddFontFromFileTTF("fonts/karla.ttf", 20.0f);
  button_font_ = io.Fonts->AddFontFromFileTTF("fonts/karla.ttf", 32.0f);
  // }

  // if (fs::is_regular_file("fonts/mononoki.ttf")) {
  logging_font_ = io.Fonts->AddFontFromFileTTF("fonts/mononoki.ttf", 20.0f);
  // }

  // if (general_font() == nullptr) {
  //   ImFontConfig font_config;
  //   font_config.SizePixels = 25.0f;

  //   general_font_ = io.Fonts->AddFontDefault(&font_config);
  // }

  // if (logging_font() == nullptr) {
  //   ImFontConfig font_config;
  //   font_config.SizePixels = 25.0f;

  //   logging_font_ = io.Fonts->AddFontDefault(&font_config);
  // }
}

void Manager::exit() {
#if defined(OPENGL3_EXIST)
  ImGui_ImplOpenGL3_Shutdown();
#elif defined(OPENGL2_EXIST)
  ImGui_ImplOpenGL2_Shutdown();
#endif
  ImGui::DestroyContext();
  if (window() != nullptr) {
    glfwDestroyWindow(window());
  }
  glfwTerminate();
  exited_ = true;
}

bool Manager::handle_events() {
  bool terminate = glfwWindowShouldClose(window());

  terminated_ = terminate;

  return !terminate;
}

void Manager::add_window(Manager::WindowPtr&& window) {
  massert(active(), "sanity");
  windows().push_back(window);
}

void Manager::render() {
  glfwPollEvents();

#if defined(OPENGL3_EXIST)
  ImGui_ImplOpenGL3_NewFrame();
#elif defined(OPENGL2_EXIST)
  ImGui_ImplOpenGL2_NewFrame();
#endif

  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::PushFont(general_font());

  // render windows
  for (auto&& s_window : windows())
    s_window->render(this);

  ImGui::PopFont();

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
  for (auto&& s_window : windows())
    s_window->after_render(this);
}
}  // namespace gui

NAMESPACE_END
