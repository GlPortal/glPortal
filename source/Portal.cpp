#include "Portal.hpp"

#include <algorithm>
#include <cmath>

#include <assets/model/MeshLoader.hpp>

#include <engine/core/math/Math.hpp>
#include <engine/Entity.hpp>
#include <engine/component/Transform.hpp>
#include <engine/physics/Uncollider.hpp>
#include "World.hpp"

namespace glPortal {

const int Portal::PORTAL_RANGE = 1000;
const Vector3f Portal::BLUE_COLOR = Vector3f(0.33, 0.57, 1);
const Vector3f Portal::ORANGE_COLOR = Vector3f(1, 0.76, 0.33);
const double Portal::NOISE_FADE_DELAY = .300;
const double Portal::OPEN_ANIM_DURATION = .250;
const float Portal::SURFACE_OFFSET = 0.01f;

Portal::Portal(Entity &ent)  : Component(ent), openSince(0), open(false) {
  uncolliderMotionState.reset(new btDefaultMotionState);
  wrapper.vertShape.reset(new btBoxShape(btVector3(0.1, 1, 0.5)));
  wrapper.horzShape.reset(new btBoxShape(btVector3(.5, 0.1, 0.5)));
  // TODO Handle collision subtraction better
  Uncollider::portals.emplace_back(&entity);
}

Portal::~Portal() {
  Uncollider::portals.erase(std::remove(
    Uncollider::portals.begin(), Uncollider::portals.end(), &entity), Uncollider::portals.end());
}

Vector3f Portal::getDirection() const {
  return direction;
}

void Portal::placeWrapperPiece(const Vector3f &p, const Quaternion &o, const Vector3f &s,
    const std::unique_ptr<btCollisionShape> &shape, Wrapper::Side &side, const Vector3f &offset) {
  side.motionState.reset(new btDefaultMotionState);
  side.motionState->setWorldTransform(
    btTransform(btQuaternion(0, 0, 0, 1), p) *
    btTransform(o) *
    btTransform(btQuaternion(0, 0, 0, 1), offset));
  btRigidBody::btRigidBodyConstructionInfo ci(0, side.motionState.get(),
    shape.get(), btVector3(0, 0, 0));
  if (side.body) {
    entity.manager.scene.physics.world->removeRigidBody(side.body.get());
  }
  side.body.reset(new btRigidBody(ci));
  entity.manager.scene.physics.world->addRigidBody(side.body.get());
}

void Portal::placeOnWall(const Vector3f &launchPos, const Vector3f &point, const Vector3f &normal) {
  //Determine on what side the portal is
  //Side 0: -x, Side 1: x, Side 2: -z, Side 3: z, Side 4: -y, Side 5: y
  /*float dist = 1000000;
  int side = 0;

  float *distances;
  distances = PortalHelper::getDistancesForPoint(point, wall);

  for (int i = 0; i < 6; i++) {
    if (distances[i] < dist) {
      side = i;
      dist = distances[i];
    }
  }*/

  Transform &t = entity.getComponent<Transform>();
  Quaternion orientation;
  Vector3f position(point);
  Vector3f scale(1, 2, 1);
  direction = normalize(normal);
  if (normal.fuzzyEqual(Vector3f::FORWARD)) {
    // Edge case, since the portal has a "default normal" of 0, 0, -1
    // the below quaternion-from-normal formula would be ambiguous
    // and possibly fail
    orientation = Quaternion();
  } else if (normal.fuzzyEqual(Vector3f(0, 0, 1))) {
    // Same as above, other side
    orientation = Quaternion(0, sin(M_PI/2), 0, cos(M_PI/2));
  } else if (normal.fuzzyEqual(Vector3f(0, 1, 0))) {
    // Floor
    float yRot = std::atan2(point.x-launchPos.x, point.z-launchPos.z);
    orientation.fromAero(0, rad(90), -yRot);
  } else if (normal.fuzzyEqual(Vector3f(0, -1, 0))) {
    // Ceiling
    float yRot = std::atan2(point.x-launchPos.x, point.z-launchPos.z);
    orientation.fromAero(0, rad(-90), yRot);
  } else {
    // http://www.gamedev.net/topic/566295-normal-to-a-quaternion/
    const Vector3f from(0, 0, -1);
    Vector3f H = normalize(from + normal);
    orientation.w = dot(from, H);
    orientation.x = from.y*H.z - from.z*H.y;
    orientation.y = from.z*H.x - from.x*H.z;
    orientation.z = from.x*H.y - from.y*H.x;
  }

  // Enable the light
  // entity.getComponent<LightSource>().enabled = true;

  placeWrapperPiece(position, orientation, scale,
    wrapper.horzShape, wrapper.bottom, Vector3f(0, -1.076, 0.501));
  placeWrapperPiece(position, orientation, scale,
    wrapper.horzShape, wrapper.top, Vector3f(0, 1.076, 0.501));
  placeWrapperPiece(position, orientation, scale,
    wrapper.vertShape, wrapper.right, Vector3f(0.576, 0, 0.501));
  placeWrapperPiece(position, orientation, scale,
    wrapper.vertShape, wrapper.left, Vector3f(-0.576, 0, 0.501));

  uncolliderMotionState->setWorldTransform(
    btTransform(btQuaternion(0, 0, 0, 1), position) *
    btTransform(orientation) *
    btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0.499)));
  uncolliderShape.reset(new btBoxShape(scale/2));
  btRigidBody::btRigidBodyConstructionInfo ci(0, uncolliderMotionState.get(),
    uncolliderShape.get(), btVector3(0, 0, 0));
  uncollider.reset(new btRigidBody(ci));

  open = true;
  position += (getDirection() * SURFACE_OFFSET);
  overlayMesh = MeshLoader::getMesh("Plane.obj");
  stencilMesh = MeshLoader::getMesh("PortalStencil.obj");

  t.setPosition(position);
  t.setOrientation(orientation);
  t.setScale(scale);
}

Vector3f Portal::getScaleMult() const {
  double delta = entity.manager.scene.world->getTime()-openSince;
  if (delta > OPEN_ANIM_DURATION) {
    return Vector3f(1, 1, 1);
  }
  float s = delta;

  // Linear:
  //  std::min((float)delta/OPEN_ANIM_DURATION, 1.0f);

  // Quadratic in/out:
  s /= OPEN_ANIM_DURATION / 2;
  if (s < 1) {
    s = 0.5f*s*s;
  } else {
    s--;
    s = -0.5f * (s*(s-2) - 1);
  }

  return Vector3f(s, s, s);
}

} /* namespace glPortal */
