#pragma once

#include "Vec2.h"

#include <span>
#include <string>

struct GLFWwindow;

namespace myimpl {

class Renderer {
public:
  Renderer();
  ~Renderer();

  bool initialize(int width, int height, const std::string &title);
  void shutdown();

  bool shouldClose() const;
  void beginFrame();
  void drawGround(double yWorld);
  void drawTrail(std::span<const Vec2> points);
  void drawProjectile(const Vec2 &position);
  void endFrame();

  GLFWwindow *window() const { return m_window; }

private:
  struct Vertex {
    float x;
    float y;
    float r;
    float g;
    float b;
  };

  unsigned int compileShader(unsigned int type, const char *source);
  bool createProgram();
  Vertex toVertex(const Vec2 &point, float r, float g, float b) const;
  void uploadAndDraw(unsigned int mode, std::span<const Vertex> vertices,
                     float pointSize = 1.0f) const;

  GLFWwindow *m_window = nullptr;
  unsigned int m_program = 0;
  unsigned int m_vao = 0;
  unsigned int m_vbo = 0;

  int m_width = 1280;
  int m_height = 720;
  double m_worldWidth = 80.0;
  double m_worldHeight = 45.0;
};

} // namespace myimpl
