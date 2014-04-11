#ifndef __BOX_HPP
#define __BOX_HPP

#include "Resources.hpp"
#include "Vertex.hpp"

#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))

namespace glPortal {
  namespace engine {

    /**
     * Class abstracting a box in space.
     * Coordinates will be sorted such that x1 <= x2, y1 <=2 etc.
     */

    class Box {
    public:
      float x1,y1,z1;
      float x2,y2,z2;
      TEXTURE_ID type; /**< Material type of the box */

      Box() : x1(0), y1(0), z1(0), x2(0), y2(0), z2(0), type(TID_NONE) {}

      /**
       * Please prefer this constructor
       */
      Box(Vertex start, Vertex end, TEXTURE_ID type = TID_NONE) {	
        set(start.getX(), start.getY(), start.getZ(), end.getX(), end.getY(), end.getZ(), type);
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
        this->x1 = MIN(x1, x2);
        this->x2 = MAX(x1, x2);

        this->y1 = MIN(y1, y2);
        this->y2 = MAX(y1, y2);

        this->z1 = MIN(z1, z2);
        this->z2 = MAX(z1, z2);

        this->type = type;
      }
	
      /**
       * Checks whether this box collides with another
       *
       * @param b The Box to collide with
       * @return True if the boxes collide
       */
      bool collide(const Box &b) {
        if(b.x1 > x2 || b.x2 < x1 ||b.y1 > y2 || b.y2 < y1 ||b.z1 > z2 || b.z2 < z1)
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
        if(x > x1 && x < x2 && y > y1 && y < y2 && z > z1 && z < z2)
          return true;
        else
          return false;
      }
    };
  }
}
#endif
