#include <glPortal/Portal.hpp>

#include <algorithm>
#include <bullet/BulletDynamics/Dynamics/btRigidBody.h>
#include <bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include <radix/data/model/MeshLoader.hpp>
#include <radix/core/math/Math.hpp>
#include <radix/Entity.hpp>
#include <radix/simulation/Physics.hpp>
#include <radix/physics/Uncollider.hpp>
#include <radix/World.hpp>

using namespace radix;

namespace glPortal {

const int Portal::PORTAL_RANGE = 1000;
const Vector3f Portal::BLUE_COLOR = Vector3f(0.33, 0.57, 1);
const Vector3f Portal::ORANGE_COLOR = Vector3f(1, 0.76, 0.33);
const double Portal::NOISE_FADE_DELAY = .300;
const double Portal::OPEN_ANIM_DURATION = .250;
const float Portal::SURFACE_OFFSET = 0.01f;

Portal::Portal(const CreationParams &cp) :
  radix::Entity(cp),
  openSince(0),
  open(false),
  trigger(nullptr) {
  uncolliderMotionState.reset(new btDefaultMotionState);
  wrapper.vertShape.reset(new btBoxShape(btVector3(0.1, 1, 0.5)));
  wrapper.horzShape.reset(new btBoxShape(btVector3(.5, 0.1, 0.5)));
  // TODO Handle collision subtraction better
}

Portal::~Portal() {
  Uncollider::volumes.remove(uncollider.get());
}

Vector3f Portal::getDirection() const {
  return direction;
}

void Portal::placeWrapperPiece(const Vector3f &p, const Quaternion &o, const Vector3f &s,
    const std::unique_ptr<btCollisionShape> &shape, Wrapper::Side &side, const Vector3f &offset) {
  side.btPtrInfo = radix::util::BulletUserPtrInfo(this);
  side.motionState.reset(new btDefaultMotionState);
  side.motionState->setWorldTransform(
    btTransform(btQuaternion(0, 0, 0, 1), p) *
    btTransform(o) *
    btTransform(btQuaternion(0, 0, 0, 1), offset));
  btRigidBody::btRigidBodyConstructionInfo ci(0, side.motionState.get(),
    shape.get(), btVector3(0, 0, 0));
  if (side.body) {
    world.simulations.findFirstOfType<simulation::Physics>()
        .getPhysicsWorld().removeRigidBody(side.body.get());
  }
  side.body.reset(new btRigidBody(ci));
  side.body->setUserPointer(&side.btPtrInfo);
  side.body->setUserIndex(id);
  world.simulations.findFirstOfType<simulation::Physics>()
      .getPhysicsWorld().addRigidBody(side.body.get());
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
    orientation = Quaternion(0, sin(Math::PI / 2), 0, cos(Math::PI / 2));
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
  Uncollider::volumes.remove(uncollider.get());
  uncollider.reset(new btRigidBody(ci));
  Uncollider::volumes.emplace_back(uncollider.get());

  open = true;
  position += (getDirection() * SURFACE_OFFSET);
  overlayMesh = MeshLoader::getMesh("Plane.obj");
  stencilMesh = MeshLoader::getMesh("PortalStencil.obj");

  setPosition(position);
  setOrientation(orientation);
  setScale(scale);
}

Vector3f Portal::getScaleMult() const {
  double delta = world.getTime() - openSince;
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
