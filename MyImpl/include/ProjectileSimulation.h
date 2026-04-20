#pragma once

#include "Vec2.h"

#include <vector>

namespace myimpl {

struct ProjectileState {
  Vec2 position;
  Vec2 velocity;
};

class ProjectileSimulation {
public:
  ProjectileSimulation();

  void reset();
  void step(double dt);

  const ProjectileState &state() const { return m_state; }
  const std::vector<Vec2> &trail() const { return m_trail; }

  bool hasLanded() const { return m_hasLanded; }
  double elapsedTime() const { return m_elapsedTime; }
  double launchSpeed() const { return m_launchSpeed; }
  double launchAngleDeg() const { return m_launchAngleDeg; }

private:
  void appendTrailPoint();

  ProjectileState m_state{};
  std::vector<Vec2> m_trail;
  bool m_hasLanded = false;
  double m_elapsedTime = 0.0;

  double m_launchSpeed = 28.0;
  double m_launchAngleDeg = 52.0;
  Vec2 m_gravity{0.0, -9.81};
  double m_groundHeight = 0.0;
};

} // namespace myimpl
