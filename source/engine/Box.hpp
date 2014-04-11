#ifndef __BOX_HPP
#define __BOX_HPP

#include "Resources.hpp"
#include "Vertex.hpp"
#include "../util/Vector3f.hpp"

#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))

using namespace util;

namespace glPortal {
  namespace engine {

    /**
     * Class abstracting a box in space.
     * Coordinates will be sorted such that x1 <= x2, y1 <=2 etc.
     */

    class Box {
    public:
      Vector3f pos;
      Vector3f size;
      
      Vector3f start;
      Vector3f end;
      
      TEXTURE_ID type; /**< Material type of the box */

      Box() : type(TID_NONE) {}

      /**
       * Please prefer this constructor
       */
      Box(Vector3f start, Vector3f end, TEXTURE_ID type = TID_NONE) {	
        set(start.x, start.y, start.z, end.x, end.y, end.z, type);
      }

      Box(float *val, TEXTURE_ID type = TID_NONE) {
        set(val[0], val[1], val[2], val[3], val[4], val[5], type);
      }

      /**
       * @deprecated This constructor is a trainwreck. Please use the smaller constructor instead.
       */
      Box(float x1, float y1, float z1, float x2, float y2, float z2, TEXTURE_ID type = TID_NONE) {
        set(x1, y1, z1, x2, y2, z2, type);
      }

      /**
       * Sets the member value sorted such that x1 <= x2, y1 <= y2 and z1 <= z2.
       */
      void set(float x1, float y1, float z1, float x2, float y2, float z2, TEXTURE_ID type = TID_NONE) {
        this->start.x = MIN(x1, x2);
        this->end.x = MAX(x1, x2);

        this->start.y = MIN(y1, y2);
        this->end.y = MAX(y1, y2);

        this->start.z = MIN(z1, z2);
        this->end.z = MAX(z1, z2);

        this->type = type;
      }
	
      /**
       * Checks whether this box collides with another
       *
       * @param b The Box to collide with
       * @return True if the boxes collide
       */
      bool collide(const Box &b) {
        if(b.start.x > end.x || b.end.x < start.x ||b.start.y > end.y || b.end.y < start.y ||b.start.z > end.z || b.end.z < start.z)
          return false;
        else
	        return true;
      }

      /**
       * Checks whether a point lies within the box
       *
       * @param x X-coordinate of the point
       * @param y Y-coordinate of the point
       * @param z Z-coordinate of the point
       * @return True if a collision occurs
       */
      bool collide(float x, float y, float z) {
        if(x > start.x && x < end.x && y > start.y && y < end.y && z > start.z && z < end.z)
          return true;
        else
          return false;
      }
    };
  }
}
#endif
