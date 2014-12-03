#include "Portal.hpp"

#include <cmath>

#include <assets/model/MeshLoader.hpp>
#include <assets/model/Mesh.hpp>

#include <engine/BoxCollider.hpp>

namespace glPortal {

const int Portal::PORTAL_RANGE = 1000;
const Vector3f Portal::BLUE_COLOR = Vector3f(0.33, 0.57, 1);
const Vector3f Portal::ORANGE_COLOR = Vector3f(1, 0.76, 0.33);

Vector3f Portal::getDirection() {
  return direction;
}

bool Portal::throughPortal(BoxCollider collider) {
  if (rotation.x == 0) {
    if (rotation.y == 90) {
      if (collider.position.x > position.x &&
         collider.position.x + collider.size.x/2 < position.x + scale.x &&
         collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
         collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
         collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
         collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
        return true;
      }
    }
    if (rotation.y == -90) {
      if (collider.position.x < position.x &&
         collider.position.x - collider.size.x/2 > position.x - scale.x &&
         collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
         collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
         collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
         collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
        return true;
      }
    }
    if (rotation.y == 0) {
      if (collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
         collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
         collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
         collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
         collider.position.z > position.z &&
         collider.position.z + collider.size.z/2 < position.z + scale.z) {
        return true;
      }
    }
    if (rotation.y == 180) {
      if (collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
         collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
         collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
         collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
         collider.position.z < position.z &&
         collider.position.z - collider.size.z/2 > position.z - scale.z) {
        return true;
      }
    }
  } else {
    if (rotation.x == -90) {
      if (collider.position.x - collider.size.x/2 >= position.x - scale.x/2 &&
         collider.position.x + collider.size.x/2 <= position.x + scale.x/2 &&
         collider.position.y > position.y &&
         collider.position.y + collider.size.y/2 <= position.y + scale.y &&
         collider.position.z - collider.size.z/2 >= position.z - scale.z/2 &&
         collider.position.z + collider.size.z/2 <= position.z + scale.z/2) {
        return true;
      }
    }
    if (rotation.x == 90) {
      if (collider.position.x - collider.size.x/2 >= position.x - scale.x/2 &&
         collider.position.x + collider.size.x/2 <= position.x + scale.x/2 &&
         collider.position.y < position.y &&
         collider.position.y - collider.size.y/2 >= position.y - scale.y &&
         collider.position.z - collider.size.z/2 >= position.z - scale.z/2 &&
         collider.position.z + collider.size.z/2 <= position.z + scale.z/2) {
        return true;
      }
    }
  }
  return false;
}

bool Portal::inPortal(BoxCollider collider) {
  if (rotation.x == 0) {
    if (rotation.y == 90) {
      if (collider.position.x + collider.size.x/2 > position.x &&
         collider.position.x + collider.size.x/2 < position.x + scale.x &&
         collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
         collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
         collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
         collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
        return true;
      }
    }
    if (rotation.y == -90) {
      if (collider.position.x - collider.size.x/2  < position.x &&
         collider.position.x - collider.size.x/2 > position.x - scale.x &&
         collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
         collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
         collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
         collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
        return true;
      }
    }
    if (rotation.y == 0) {
      if (collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
         collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
         collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
         collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
         collider.position.z + collider.size.z/2 > position.z &&
         collider.position.z + collider.size.z/2 < position.z + scale.z) {
        return true;
      }
    }
    if (rotation.y == 180) {
      if (collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
         collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
         collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
         collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
         collider.position.z - collider.size.z/2 < position.z &&
         collider.position.z - collider.size.z/2 > position.z - scale.z) {
        return true;
      }
    }
  } else {
    if (rotation.x == -90) {
      if (collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
         collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
         collider.position.y + collider.size.y/2 > position.y &&
         collider.position.y + collider.size.y/2 < position.y + scale.y &&
         collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
         collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
        return true;
      }
    }
    if (rotation.x == 90) {
      if (collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
         collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
         collider.position.y - collider.size.y/2 < position.y &&
         collider.position.y - collider.size.y/2 > position.y - scale.y &&
         collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
         collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
        return true;
      }
    }
  }
  return false;
}

void Portal::placeOnWall(BoxCollider wall, Vector3f point) {
  //Determine on what side the portal is
  //Side 0: -x, Side 1: x, Side 2: -z, Side 3: z, Side 4: -y, Side 5: y
  float dist = 1000000;
  int side = 0;
  float distances[6] = {(float) fabs(point.x - (wall.position.x - wall.size.x / 2)),
                        (float) fabs(point.x - (wall.position.x + wall.size.x / 2)),
                        (float) fabs(point.z - (wall.position.z - wall.size.z / 2)),
                        (float) fabs(point.z - (wall.position.z + wall.size.z / 2)),
                        (float) fabs(point.y - (wall.position.y - wall.size.y / 2)),
                        (float) fabs(point.y - (wall.position.y + wall.size.y / 2))};

  for (int i = 0; i < 6; i++) {
    if (distances[i] < dist) {
      side = i;
      dist = distances[i];
    }
  }

  position.set(point);

  switch (side) {
  case 0: direction.set(-1, 0, 0); break;
  case 1: direction.set(1, 0, 0); break;
  case 2: direction.set(0, 0, -1); break;
  case 3: direction.set(0, 0, 1); break;
  case 4: direction.set(0, -1, 0); break;
  case 5: direction.set(0, 1, 0); break;
  }

  if (wall.size.z >= 1 && wall.size.y >= 2 && (side == 0 || side == 1)) {
    if (side == 0) {
      rotation.y = 90;
      scale.set(1, 2, 1);
    }
    if (side == 1) {
      rotation.y = -90;
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
    if (side == 2) {
      rotation.y = 0;
      scale.set(1, 2, 1);
    }
    if (side == 3) {
      rotation.y = 180;
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
    if (side == 4) {
      rotation.x = -90;
      scale.set(1, 2, 2);
    }
    if (side == 5) {
      rotation.x = 90;
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

  position.add(*getDirection().scale(0.01f));
  mesh = MeshLoader::getMesh("Plane.obj");
}

} /* namespace glPortal */
