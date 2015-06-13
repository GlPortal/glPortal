#include "Ray.hpp"

#include "Entity.hpp"
#include <engine/component/Transform.hpp>
#include <climits>

namespace glPortal {

bool Ray::collides(const BoxCollider &c, float *tNear, float *tFar) {
  const Vector3f &position = c.position;
  const Vector3f &scale = c.size;

  float min[3] = {position.x - scale.x / 2,
                  position.y - scale.y / 2,
                  position.z - scale.z / 2};
  float max[3] = {position.x + scale.x / 2,
                  position.y + scale.y / 2,
                  position.z + scale.z / 2};

  float ro[3] = { origin.x, origin.y, origin.z };
  float rd[3] = { direction.x, direction.y, direction.z };

  *tNear = INT_MIN;
  *tFar = INT_MAX;

  for (int i = 0; i < 3; i++) {
    //Parallel lines
    if (rd[i] == 0) {
      if (ro[i] < min[i] || ro[i] > max[i]) {
        return false;
      }
    } else {
      //Slab intersection
      float t1 = (min[i] - ro[i]) / rd[i];
      float t2 = (max[i] - ro[i]) / rd[i];
      //If t1 is bigger than t2 switch them
      if (t1 > t2) {
        float temp = t1;
        t1 = t2;
        t2 = temp;
      }
      if (t1 > *tNear) { *tNear = t1; }
      if (t2 < *tFar) { *tFar = t2; }

      //End
      if (*tNear > *tFar) { return false; }
      if (*tFar < 0) { return false; }
    }
  }
  return true;
}

} /* namespace glPortal */
