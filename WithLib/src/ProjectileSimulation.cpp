#include "ProjectileSimulation.h"

#include "PhysicsBody.h"
#include "ballistics/external/PhysicsWorld.h"
#include "ballistics/external/forces/Gravity.h"
#include "builtin/bodies/RigidBody.h"
#include "geography/CoordinateMapping.h"
#include "math/Integrator.h"

#include <cmath>
#include <memory>

namespace withlib {

namespace {

double degToRad(double degrees) { return degrees * std::acos(-1.0) / 180.0; }

BulletPhysics::math::Vec3 makeVelocity(double speed, double angleDeg) {
  const double angleRad = degToRad(angleDeg);
  return {
      speed * std::cos(angleRad),
      speed * std::sin(angleRad),
      0.0,
  };
}

BulletPhysics::math::Vec3 makePosition(double x, double y) {
  return {x, y, 0.0};
}

Vec2 toVec2(const BulletPhysics::math::Vec3 &value) {
  return {value.x, value.y};
}

std::unique_ptr<BulletPhysics::builtin::bodies::RigidBody> makeBody() {
  auto body = std::make_unique<BulletPhysics::builtin::bodies::RigidBody>();
  body->setMass(1.0);
  return body;
}

std::unique_ptr<BulletPhysics::math::MidpointIntegrator> makeIntegrator() {
  return std::make_unique<BulletPhysics::math::MidpointIntegrator>();
}

std::unique_ptr<BulletPhysics::ballistics::external::PhysicsWorld> makeWorld() {
  auto world =
      std::make_unique<BulletPhysics::ballistics::external::PhysicsWorld>();
  world->addForce(
      std::make_unique<BulletPhysics::ballistics::external::forces::Gravity>());
  return world;
}

} // namespace

ProjectileSimulation::ProjectileSimulation() {
  BulletPhysics::geography::CoordinateMapping::set(
      BulletPhysics::geography::mappings::OpenGL());
  m_body = makeBody();
  m_integrator = makeIntegrator();
  m_world = makeWorld();

  reset();
}

ProjectileSimulation::~ProjectileSimulation() = default;

void ProjectileSimulation::reset() {
  m_body->setPosition(makePosition(0.0, 0.0));
  m_body->setVelocity(makeVelocity(m_launchSpeed, m_launchAngleDeg));

  m_hasLanded = false;
  m_elapsedTime = 0.0;
  m_trail.clear();
  m_trail.reserve(2048);

  syncStateFromBody();
  appendTrailPoint();
}

void ProjectileSimulation::step(double dt) {
  if (m_hasLanded) {
    return;
  }

  m_integrator->step(*m_body, m_world.get(), dt);
  m_elapsedTime += dt;
  syncStateFromBody();

  if (m_state.position.y <= m_groundHeight) {
    m_body->setPosition(makePosition(m_state.position.x, m_groundHeight));
    m_body->setVelocity(makePosition(0.0, 0.0));
    m_hasLanded = true;
    syncStateFromBody();
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

void ProjectileSimulation::syncStateFromBody() {
  m_state.position = toVec2(m_body->getPosition());
  m_state.velocity = toVec2(m_body->getVelocity());
}

} // namespace withlib
