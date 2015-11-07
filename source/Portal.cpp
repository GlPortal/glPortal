#include "Portal.hpp"

#include <cmath>

#include <assets/model/MeshLoader.hpp>

#include <engine/BoxCollider.hpp>
#include <engine/core/math/Math.hpp>
#include <engine/Entity.hpp>
#include <engine/component/Transform.hpp>
#include "PortalHelper.hpp"
#include "World.hpp"

namespace glPortal {

const int Portal::PORTAL_RANGE = 1000;
const Vector3f Portal::BLUE_COLOR = Vector3f(0.33, 0.57, 1);
const Vector3f Portal::ORANGE_COLOR = Vector3f(1, 0.76, 0.33);
const double Portal::NOISE_FADE_DELAY = .300;
const double Portal::OPEN_ANIM_DURATION = .250;
const float Portal::SURFACE_OFFSET = 0.01f;

Portal::Portal(Entity &ent)  : Component(ent), open(false) {
  uncolliderMotionState.reset(new btDefaultMotionState);
}

Vector3f Portal::getDirection() const {
  return direction;
}

bool Portal::inPortal(const BoxCollider &collider) const {
  
  return PortalHelper::isInPortal(entity, collider);
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
  Quaternion &orientation = t.orientation;
  Vector3f &position = t.position;
  Vector3f &scale = t.scale;
  scale = Vector3f(1, 2, 1);
  position = point;
  direction = normal;
  if (normal.fuzzyEqual(Vector3f(0, 0, -1))) {
    // Edge case, since the portal has a "default normal" of 0, 0, -1
    // the below quaternion-from-normal formula would be ambiguous
    // and possibly fail
    orientation = Quaternion();
  } else if (normal.fuzzyEqual(Vector3f(0, 0, 1))) {
    // Same as above, other side
    orientation = Quaternion(0, sin(M_PI/2), 0, cos(M_PI/2));
    //Quaternion(0, sin(rad(180)/2), 0, cos(rad(180)/2));
  } else if (normal.fuzzyEqual(Vector3f(0, 1, 0))) {
    // Floor
    float yRot = std::atan2(point.x-launchPos.x, point.z-launchPos.z);
    orientation.setFromEuler(rad(-0), yRot, rad(90));
  } else if (normal.fuzzyEqual(Vector3f(0, -1, 0))) {
    // Ceiling
    float yRot = std::atan2(point.x-launchPos.x, point.z-launchPos.z);
    orientation.setFromEuler(rad(-0), yRot, rad(-90));
  } else {
    // http://www.gamedev.net/topic/566295-normal-to-a-quaternion/
    const Vector3f from(0, 0, -1);
    Vector3f H = normalize(from + normal);
    orientation.w = dot(from, H);
    orientation.x = from.y*H.z - from.z*H.y;
    orientation.y = from.z*H.x - from.x*H.z; 
    orientation.z = from.x*H.y - from.y*H.x;
  }

  uncolliderMotionState->setWorldTransform(btTransform(orientation, position));
  uncolliderShape.reset(new btBoxShape(scale/2));
  btRigidBody::btRigidBodyConstructionInfo ci(0, uncolliderMotionState.get(), uncolliderShape.get(), btVector3(0, 0, 0));
  uncollider.reset(new btRigidBody(ci));

  open = true;
  position += (getDirection() * SURFACE_OFFSET);
  overlayMesh = MeshLoader::getMesh("Plane.obj");
  stencilMesh = MeshLoader::getMesh("PortalStencil.obj");
}

Vector3f Portal::getScaleMult() const {
  double delta = entity.manager.scene.world->getTime()-openSince;
  if (delta > OPEN_ANIM_DURATION)
    return Vector3f(1, 1, 1);
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
