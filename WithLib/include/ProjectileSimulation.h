#pragma once

#include "Vec2.h"

#include <memory>
#include <vector>

namespace BulletPhysics {
namespace builtin {
namespace bodies {
class RigidBody;
}
} // namespace builtin
namespace math {
class MidpointIntegrator;
}
namespace ballistics {
namespace external {
class PhysicsWorld;
}
} // namespace ballistics
namespace geography {
class CoordinateMapping;
}
} // namespace BulletPhysics

namespace withlib {

struct ProjectileState {
  Vec2 position;
  Vec2 velocity;
};

class ProjectileSimulation {
public:
  ProjectileSimulation();
  ~ProjectileSimulation();

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
  void syncStateFromBody();

  ProjectileState m_state{};
  std::vector<Vec2> m_trail;
  bool m_hasLanded = false;
  double m_elapsedTime = 0.0;

  double m_launchSpeed = 28.0;
  double m_launchAngleDeg = 52.0;
  double m_groundHeight = 0.0;

  std::unique_ptr<BulletPhysics::builtin::bodies::RigidBody> m_body;
  std::unique_ptr<BulletPhysics::math::MidpointIntegrator> m_integrator;
  std::unique_ptr<BulletPhysics::ballistics::external::PhysicsWorld> m_world;
};

} // namespace withlib
