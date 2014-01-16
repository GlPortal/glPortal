#ifndef __VERTEX_HPP
#define __VERTEX_HPP

namespace glPortal {
  namespace engine {

    /**
     * Class abstracting a vertex in space.
     */

    class Vertex {
    public:
      float x,y,z;
      Vertex() : x(0), y(0), z(0){}

      Vertex(float x, float y, float z) {
	set(x, y, z);
      }

      Vertex(float *val) {
	set(val[0], val[1], val[2]);
      }

      void set(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
      }	

      float getX() {
	return this->x;
      }	

      float getY() {
	return this->y;
      }	

      float getZ() {
	return this->z;
      }	
    };
  }
}
#endif
