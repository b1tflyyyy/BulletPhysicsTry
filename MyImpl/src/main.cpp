#include "ProjectileSimulation.h"
#include "Renderer.h"

#include <GLFW/glfw3.h>

int main() {
  myimpl::Renderer renderer;
  if (!renderer.initialize(1280, 720, "MyImpl")) {
    return 1;
  }

  myimpl::ProjectileSimulation simulation;
  bool paused = false;
  bool resetPressedLastFrame = false;
  bool pausePressedLastFrame = false;

  constexpr double dt = 1.0 / 240.0;

  while (!renderer.shouldClose()) {
    GLFWwindow *window = renderer.window();

    const bool escapePressed =
        glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    if (escapePressed) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    const bool resetPressed = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
    if (resetPressed && !resetPressedLastFrame) {
      simulation.reset();
      paused = false;
    }
    resetPressedLastFrame = resetPressed;

    const bool pausePressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    if (pausePressed && !pausePressedLastFrame) {
      paused = !paused;
    }
    pausePressedLastFrame = pausePressed;

    if (!paused) {
      simulation.step(dt);
    }

    renderer.beginFrame();
    renderer.drawGround(0.0);
    renderer.drawTrail(simulation.trail());
    renderer.drawProjectile(simulation.state().position);
    renderer.endFrame();
  }

  return 0;
}
