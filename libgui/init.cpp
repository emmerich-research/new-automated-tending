#include "gui.hpp"

#include "init.hpp"

#include <exception>
#include <iostream>

NAMESPACE_BEGIN

GLFWwindow* window_ = nullptr;

static void glfw_error_callback(int error, const char* description) {
  std::cerr << "Glfw Error " << error << ": " << description << std::endl;
  // LOG_DEBUG("Glfw Error {}: {}", error, description);
}

ATM_STATUS initialize_gui() {
  massert(Config::get() != nullptr, "config is not initialized");
  massert(Logger::get() != nullptr, "logger is not initialized");

  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return ATM_ERR;

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

  window_ = glfwCreateWindow(1024, 768, Config::get()->name().c_str(),
                             glfwGetPrimaryMonitor(), NULL);
  if (window() == nullptr) {
    return ATM_ERR;
  }
  glfwMakeContextCurrent(window());
  glfwSwapInterval(1);  // Enable vsync

#if defined(OPENGL3_EXIST)
  if (gladLoadGL() == 0) {
    std::cerr << "Failed to initialize OpenGL loader!" << std::endl;
    return ATM_ERR;
  }
#endif

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
  // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window(), true);

#if defined(OPENGL3_EXIST)
  ImGui_ImplOpenGL3_Init(glsl_version);
#elif defined(OPENGL2_EXIST)
  ImGui_ImplOpenGL2_Init();
#endif

  ImFontConfig font_config;
  font_config.SizePixels = 20.0f;
  // FontData = NULL;
  // FontDataSize = 0;
  // FontDataOwnedByAtlas = true;
  // FontNo = 0;
  // SizePixels = 0.0f;
  // OversampleH = 3; // FIXME: 2 may be a better default?
  // OversampleV = 1;
  // PixelSnapH = false;
  // GlyphExtraSpacing = ImVec2(0.0f, 0.0f);
  // GlyphOffset = ImVec2(0.0f, 0.0f);
  // GlyphRanges = NULL;
  // GlyphMinAdvanceX = 0.0f;
  // GlyphMaxAdvanceX = FLT_MAX;
  // MergeMode = false;
  // RasterizerFlags = 0x00;
  // RasterizerMultiply = 1.0f;
  // EllipsisChar = (ImWchar)-1;
  // memset(Name, 0, sizeof(Name));
  // DstFont = NULL;

  io.Fonts->AddFontDefault(&font_config);

  return ATM_OK;
}

void destroy_gui() {
  // Cleanup
#if defined(OPENGL3_EXIST)
  ImGui_ImplOpenGL3_Shutdown();
#elif defined(OPENGL2_EXIST)
  ImGui_ImplOpenGL2_Shutdown();
#endif
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window());
  glfwTerminate();
}

NAMESPACE_END
