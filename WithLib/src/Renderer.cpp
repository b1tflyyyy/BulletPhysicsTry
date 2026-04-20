#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

namespace withlib {

namespace {

constexpr const char *kVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec3 inColor;

out vec3 fragColor;

void main()
{
    gl_Position = vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
}
)";

constexpr const char *kFragmentShaderSource = R"(
#version 330 core
in vec3 fragColor;

out vec4 outColor;

void main()
{
    outColor = vec4(fragColor, 1.0);
}
)";

} // namespace

Renderer::Renderer() = default;

Renderer::~Renderer() { shutdown(); }

bool Renderer::initialize(int width, int height, const std::string &title) {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (m_window == nullptr) {
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(1);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW\n";
    shutdown();
    return false;
  }

  glViewport(0, 0, width, height);

  if (!createProgram()) {
    shutdown();
    return false;
  }

  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);

  return true;
}

void Renderer::shutdown() {
  if (m_vbo != 0) {
    glDeleteBuffers(1, &m_vbo);
    m_vbo = 0;
  }

  if (m_vao != 0) {
    glDeleteVertexArrays(1, &m_vao);
    m_vao = 0;
  }

  if (m_program != 0) {
    glDeleteProgram(m_program);
    m_program = 0;
  }

  if (m_window != nullptr) {
    glfwDestroyWindow(m_window);
    m_window = nullptr;
  }

  glfwTerminate();
}

bool Renderer::shouldClose() const {
  return m_window == nullptr || glfwWindowShouldClose(m_window);
}

void Renderer::beginFrame() {
  glClearColor(0.08f, 0.09f, 0.12f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawGround(double yWorld) {
  const std::vector<Vertex> vertices{
      toVertex({-2.0, yWorld}, 0.55f, 0.55f, 0.55f),
      toVertex({m_worldWidth, yWorld}, 0.55f, 0.55f, 0.55f),
  };

  uploadAndDraw(GL_LINES, vertices);
}

void Renderer::drawTrail(std::span<const Vec2> points) {
  if (points.size() < 2) {
    return;
  }

  std::vector<Vertex> vertices;
  vertices.reserve(points.size());

  for (const Vec2 &point : points) {
    vertices.push_back(toVertex(point, 0.95f, 0.75f, 0.20f));
  }

  uploadAndDraw(GL_LINE_STRIP, vertices);
}

void Renderer::drawProjectile(const Vec2 &position) {
  const std::vector<Vertex> vertices{
      toVertex(position, 0.95f, 0.28f, 0.18f),
  };

  uploadAndDraw(GL_POINTS, vertices, 10.0f);
}

void Renderer::endFrame() {
  glfwSwapBuffers(m_window);
  glfwPollEvents();
}

unsigned int Renderer::compileShader(unsigned int type, const char *source) {
  const unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  int success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    int logLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    std::string log(logLength, '\0');
    glGetShaderInfoLog(shader, logLength, nullptr, log.data());
    std::cerr << "Shader compilation failed: " << log << '\n';
    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

bool Renderer::createProgram() {
  const unsigned int vertexShader =
      compileShader(GL_VERTEX_SHADER, kVertexShaderSource);
  const unsigned int fragmentShader =
      compileShader(GL_FRAGMENT_SHADER, kFragmentShaderSource);
  if (vertexShader == 0 || fragmentShader == 0) {
    if (vertexShader != 0) {
      glDeleteShader(vertexShader);
    }
    if (fragmentShader != 0) {
      glDeleteShader(fragmentShader);
    }
    return false;
  }

  m_program = glCreateProgram();
  glAttachShader(m_program, vertexShader);
  glAttachShader(m_program, fragmentShader);
  glLinkProgram(m_program);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  int success = 0;
  glGetProgramiv(m_program, GL_LINK_STATUS, &success);
  if (success == GL_FALSE) {
    int logLength = 0;
    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);
    std::string log(logLength, '\0');
    glGetProgramInfoLog(m_program, logLength, nullptr, log.data());
    std::cerr << "Program link failed: " << log << '\n';
    glDeleteProgram(m_program);
    m_program = 0;
    return false;
  }

  return true;
}

Renderer::Vertex Renderer::toVertex(const Vec2 &point, float r, float g,
                                    float b) const {
  const float x = static_cast<float>((point.x / m_worldWidth) * 2.0 - 1.0);
  const float y = static_cast<float>((point.y / m_worldHeight) * 2.0 - 1.0);
  return {x, y, r, g, b};
}

void Renderer::uploadAndDraw(unsigned int mode,
                             std::span<const Vertex> vertices,
                             float pointSize) const {
  if (vertices.empty()) {
    return;
  }

  glUseProgram(m_program);
  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size_bytes()),
               vertices.data(), GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<const void *>(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<const void *>(sizeof(float) * 2));
  glEnableVertexAttribArray(1);

  if (mode == GL_POINTS) {
    glPointSize(pointSize);
  }

  glDrawArrays(mode, 0, static_cast<GLsizei>(vertices.size()));
}

} // namespace withlib
