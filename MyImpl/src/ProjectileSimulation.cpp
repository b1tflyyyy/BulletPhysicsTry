#include "ProjectileSimulation.h"

#include <algorithm>

namespace myimpl {

ProjectileSimulation::ProjectileSimulation() { reset(); }

void ProjectileSimulation::reset() {
  const double angleRad = degToRad(m_launchAngleDeg);

  m_state.position = {0.0, 0.0};
  m_state.velocity = {
      m_launchSpeed * std::cos(angleRad),
      m_launchSpeed * std::sin(angleRad),
  };

  m_hasLanded = false;
  m_elapsedTime = 0.0;
  m_trail.clear();
  m_trail.reserve(2048);
  appendTrailPoint();
}

void ProjectileSimulation::step(double dt) {
  if (m_hasLanded) {
    return;
  }

  m_state.velocity += m_gravity * dt;
  m_state.position += m_state.velocity * dt;
  m_elapsedTime += dt;

  if (m_state.position.y <= m_groundHeight) {
    m_state.position.y = m_groundHeight;
    m_hasLanded = true;
  }

  appendTrailPoint();
}

void ProjectileSimulation::appendTrailPoint() {
  if (!m_trail.empty()) {
    const Vec2 &last = m_trail.back();
    if (std::abs(last.x - m_state.position.x) < 0.02 &&
        std::abs(last.y - m_state.position.y) < 0.02) {
      return;
    }
  }

  m_trail.push_back(m_state.position);
}

} // namespace myimpl
