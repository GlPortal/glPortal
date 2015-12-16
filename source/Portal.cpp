#include "Portal.hpp"

#include <cmath>

#include <assets/model/MeshLoader.hpp>

#include <engine/BoxCollider.hpp>
#include <engine/core/math/Math.hpp>
#include <engine/Entity.hpp>
#include <engine/component/Transform.hpp>
#include <engine/component/LightSource.hpp>
#include <PortalHelper.hpp>
#include <SDL2/SDL_timer.h>

namespace glPortal {

const int Portal::PORTAL_RANGE = 1000;
const Vector3f Portal::BLUE_COLOR = Vector3f(0.33, 0.57, 1);
const Vector3f Portal::ORANGE_COLOR = Vector3f(1, 0.76, 0.33);
const int Portal::NOISE_FADE_DELAY = 300;
const int Portal::OPEN_ANIM_DURATION = 250;
const float Portal::SURFACE_OFFSET = 0.01f;

Portal::Portal(Entity &ent): Component(ent), open(false), openSince(0),
                             overlayMesh(), stencilMesh() {
}

Vector3f Portal::getDirection() const {
  return direction;
}

bool Portal::throughPortal(const BoxCollider &collider) const {
  return PortalHelper::goesThroughPortal(entity, collider);
}

bool Portal::inPortal(const BoxCollider &collider) const {
  return PortalHelper::isInPortal(entity, collider);
}

void Portal::placeOnWall(const Vector3f &launchPos, const BoxCollider &wall,
                         const Vector3f &point) {
  //Determine on what side the portal is
  //Side 0: -x, Side 1: x, Side 2: -z, Side 3: z, Side 4: -y, Side 5: y
  float dist = 1000000;
  int side = 0;

  float *distances;
  distances = PortalHelper::getDistancesForPoint(point, wall);

  for (int i = 0; i < 6; i++) {
    if (distances[i] < dist) {
      side = i;
      dist = distances[i];
    }
  }

  Transform &t = entity.getComponent<Transform>();
  Vector3f &rotation = t.rotation;
  Vector3f &position = t.position;
  Vector3f &scale = t.scale;
  position = point;

  switch (side) {
  case 0: direction.set(-1,  0,  0); break;
  case 1: direction.set( 1,  0,  0); break;
  case 2: direction.set( 0,  0, -1); break;
  case 3: direction.set( 0,  0,  1); break;
  case 4: direction.set( 0, -1,  0); break;
  case 5: direction.set( 0,  1,  0); break;
  }

  if (wall.size.z >= 1 && wall.size.y >= 2 && (side == 0 || side == 1)) {
    rotation.x = 0;
    if (side == 0) {
      rotation.y = rad(90);
      scale.set(1, 2, 1);
    }
    if (side == 1) {
      rotation.y = rad(-90);
      scale.set(1, 2, 1);
    }
    if (position.z - scale.z/2 < wall.position.z - wall.size.z/2) {
      position.z = wall.position.z - wall.size.z/2 + scale.z/2;
    }
    if (position.z + scale.z/2 > wall.position.z + wall.size.z/2) {
      position.z = wall.position.z + wall.size.z/2 - scale.z/2;
    }
    if (position.y - scale.y/2 < wall.position.y - wall.size.y/2) {
      position.y = wall.position.y - wall.size.y/2 + scale.y/2;
    }
    if (position.y + scale.y/2 > wall.position.y + wall.size.y/2) {
      position.y = wall.position.y + wall.size.y/2 - scale.y/2;
    }
    open = true;
  }
  if (wall.size.x >= 1 && wall.size.y >= 2 && (side == 2 || side == 3)) {
    rotation.x = 0;
    if (side == 2) {
      rotation.y = 0;
      scale.set(1, 2, 1);
    }
    if (side == 3) {
      rotation.y = rad(180);
      scale.set(1, 2, 1);
    }
    if (position.x - scale.x/2 < wall.position.x - wall.size.x/2) {
      position.x = wall.position.x - wall.size.x/2 + scale.x/2;
    }
    if (position.x + scale.x/2 > wall.position.x + wall.size.x/2) {
      position.x = wall.position.x + wall.size.x/2 - scale.x/2;
    }
    if (position.y - scale.y/2 < wall.position.y - wall.size.y/2) {
      position.y = wall.position.y - wall.size.y/2 + scale.y/2;
    }
    if (position.y + scale.y/2 > wall.position.y + wall.size.y/2) {
      position.y = wall.position.y + wall.size.y/2 - scale.y/2;
    }
    open = true;
  }
  if (wall.size.x >= 1 && wall.size.z >= 2 && (side == 4 || side == 5)) {
    rotation.y = std::atan2(point.x-launchPos.x, point.z-launchPos.z);
    if (side == 4) {
      rotation.x = rad(-90);
      scale.set(1, 2, 2);
    }
    if (side == 5) {
      rotation.x = rad(90);
      scale.set(1, 2, 2);
    }
    if (position.x - scale.x/2 < wall.position.x - wall.size.x/2) {
      position.x = wall.position.x - wall.size.x/2 + scale.x/2;
    }
    if (position.x + scale.x/2 > wall.position.x + wall.size.x/2) {
      position.x = wall.position.x + wall.size.x/2 - scale.x/2;
    }
    if (position.z - scale.z/2 < wall.position.z - wall.size.z/2) {
      position.z = wall.position.z - wall.size.z/2 + scale.z/2;
    }
    if (position.z + scale.z/2 > wall.position.z + wall.size.z/2) {
      position.z = wall.position.z + wall.size.z/2 - scale.z/2;
    }
    open = true;
  }

  // Enabled the light
  entity.getComponent<LightSource>().enabled = true;

  position += (getDirection() * SURFACE_OFFSET);
  overlayMesh = MeshLoader::getMesh("Plane.obj");
  stencilMesh = MeshLoader::getMesh("PortalStencil.obj");
}

Vector3f Portal::getScaleMult() const {
  uint32_t delta  = SDL_GetTicks()-openSince;
  if (delta > OPEN_ANIM_DURATION){

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
