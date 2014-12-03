#include "Ray.hpp"

#include "Entity.hpp"
#include <climits>

namespace glPortal {

bool Ray::collides(Entity e, float* tNear, float* tFar) {
  float min[3] = {e.position.x - e.scale.x / 2,
                  e.position.y - e.scale.y / 2,
                  e.position.z - e.scale.z / 2};
  float max[3] = {e.position.x + e.scale.x / 2,
                  e.position.y + e.scale.y / 2,
                  e.position.z + e.scale.z / 2};

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
